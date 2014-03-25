/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "aalvideorenderercontrol.h"
#include "aalmediaplayercontrol.h"
#include "aalmediaplayerservice.h"

#include <qtubuntu_media_signals.h>

#include <QAbstractVideoBuffer>
#include <QAbstractVideoSurface>
#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QTimer>
#include <QUrl>
#include <QVideoSurfaceFormat>
#include <QWindow>

#include <qgl.h>

class AalGLTextureBuffer : public QAbstractVideoBuffer
{
public:
    AalGLTextureBuffer(GLuint textureId) :
        QAbstractVideoBuffer(QAbstractVideoBuffer::GLTextureHandle),
        m_textureId(textureId)
    {
    }

    MapMode mapMode() const { return NotMapped; }
    uchar *map(MapMode mode, int *numBytes, int *bytesPerLine)
    {
        qDebug() << Q_FUNC_INFO;
        Q_UNUSED(mode);
        Q_UNUSED(numBytes);
        Q_UNUSED(bytesPerLine);

        return NULL;
    }

    void unmap()
    {
        qDebug() << Q_FUNC_INFO;
    }

    QVariant handle() const
    {
        return QVariant::fromValue<unsigned int>(m_textureId);
    }

    GLuint textureId() { return m_textureId; }

private:
    GLuint m_textureId;
};

AalVideoRendererControl::AalVideoRendererControl(AalMediaPlayerService *service, QObject *parent)
   : QVideoRendererControl(parent),
     m_surface(0),
     m_service(service),
     m_textureBuffer(0),
     m_textureId(0),
     m_height(720),
     m_width(1280),
     m_firstFrame(true),
     m_secondFrame(false)
{
    qDebug() << Q_FUNC_INFO;
    m_service->setVideoSizeCb(AalVideoRendererControl::setVideoSizeCb, static_cast<void *>(this));

    // Get notified when qtvideo-node creates a GL texture
    connect(SharedSignal::instance(), SIGNAL(textureCreated(unsigned int)), this, SLOT(onTextureCreated(unsigned int)));
    connect(SharedSignal::instance(), SIGNAL(glConsumerSet()), this, SLOT(onGLConsumerSet()));
    // Get notified when the AalMediaPlayerService is ready to play video
    //connect(m_service, SIGNAL(serviceReady()), this, SLOT(onServiceReady()));
}

AalVideoRendererControl::~AalVideoRendererControl()
{
    if (m_textureBuffer) {
        GLuint textureId = m_textureBuffer->handle().toUInt();
        glDeleteTextures(1, &textureId);
        delete m_textureBuffer;
    }
}

QAbstractVideoSurface *AalVideoRendererControl::surface() const
{
    return m_surface;
}

void AalVideoRendererControl::setSurface(QAbstractVideoSurface *surface)
{
    if (m_surface != surface) {
        m_surface = surface;
        Q_EMIT surfaceChanged(surface);
    }
}

void AalVideoRendererControl::updateVideoTextureCb(void *context)
{
    Q_ASSERT(context != NULL);
    QMetaObject::invokeMethod(static_cast<AalVideoRendererControl*>(context), "updateVideoTexture", Qt::QueuedConnection);
}

void AalVideoRendererControl::setVideoSizeCb(int height, int width, void *data)
{
    if (data != NULL)
        static_cast<AalVideoRendererControl *>(data)->setVideoSize(height, width);
    else
        qWarning() << "Failed to call setVideoSize() since data is NULL." << endl;
}

void AalVideoRendererControl::setupSurface()
{
    qDebug() << Q_FUNC_INFO;

    if (!m_textureBuffer)
        m_textureBuffer = new AalGLTextureBuffer(m_textureId);

    updateVideoTexture();

    m_service->setVideoTextureNeedsUpdateCb(AalVideoRendererControl::updateVideoTextureCb, static_cast<void *>(this));
}

void AalVideoRendererControl::setVideoSize(int height, int width)
{
    m_height = height;
    m_width = width;
}

void AalVideoRendererControl::updateVideoTexture()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_surface) {
        qWarning() << "m_surface is NULL, can't update video texture" << endl;
        return;
    }

    if (!m_textureBuffer) {
        qWarning() << "m_textureBuffer is NULL, can't update video texture" << endl;
        return;
    }

    // If this is the first video frame being rendered, it's ok that m_textureId == 0.
    // This is necessary so that a ShaderVideoNode instance from qtvideo-node gets created,
    // as it is responsible for creating and returning a new texture and ID respectively.
    if (m_textureId == 0 && !m_firstFrame) {
        qWarning() << "m_textureId == 0, can't update video texture" << endl;
        return;
    }

    QVideoFrame frame(new AalGLTextureBuffer(m_textureId), QSize(m_width, m_height), QVideoFrame::Format_RGB32);
    if (!frame.isValid()) {
        qWarning() << "Frame is invalid, not presenting." << endl;
        return;
    }

    if (m_firstFrame) {
        qDebug() << "Presenting frame to get texture-id from qt-video node";
        // If this is the first video frame, we need to get a texture id from qtvideo-node
        // setMetaData triggers qtvideo-node to generate a texture id
        frame.setMetaData("GetTextureId", QVariant(0));
        m_firstFrame = false;
        m_secondFrame = true;
    }
    else if (m_secondFrame) {
        qDebug() << "Presenting frame to set GLConsumer on qtvideo-node glConsumer: " << m_service->glConsumer();
        frame.setMetaData("GLConsumer", QVariant::fromValue((unsigned int)m_service->glConsumer()));
        m_secondFrame = false;
    }

    presentVideoFrame(frame);
}

void AalVideoRendererControl::onTextureCreated(unsigned int textureID)
{
    qDebug() << __PRETTY_FUNCTION__ << ": textureId: " << textureID;
    if (m_textureId == 0) {
        m_textureId = static_cast<GLuint>(textureID);
        qDebug() << "Creating video sink!";
        m_service->createVideoSink(textureID);

        //qDebug() << "Calling updateVideoTexture()";
        // This call will make sure the GLConsumerWrapperHybris gets set on qtvideo-node
        //updateVideoTexture();

        //m_service->play();
    }
    else
        qDebug() << "Already have a texture id and video sink, not creating a new one";
}

void AalVideoRendererControl::onServiceReady()
{
    qDebug() << __PRETTY_FUNCTION__ << " - Service is ready!";
    m_textureBuffer = new AalGLTextureBuffer(m_textureId);
    setupSurface();
}

void AalVideoRendererControl::onGLConsumerSet()
{
    qDebug() << Q_FUNC_INFO;
    m_service->play();
}

void AalVideoRendererControl::presentVideoFrame(const QVideoFrame &frame, bool empty)
{
    Q_UNUSED(empty);
    Q_ASSERT(m_surface != NULL);

    qDebug() << Q_FUNC_INFO;

    if (!m_surface->isActive()) {
        QVideoSurfaceFormat format(frame.size(), frame.pixelFormat(), frame.handleType());

        if (!m_surface->start(format)) {
            qWarning() << "Failed to start video surface with format:" << format;
        }
    }

    if (m_surface->isActive()) {
        qDebug() << "m_surface->present(): frame: " << frame;
        qDebug() << "m_firstFrame: " << m_firstFrame << " m_secondFrame: " << m_secondFrame;
        m_surface->present(frame);
    }
}
