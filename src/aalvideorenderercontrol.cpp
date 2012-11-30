/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Jim Hodapp <jim.hodapp@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "aalvideorenderercontrol.h"
#include "aalmediaplayercontrol.h"
#include "aalmediaplayerservice.h"

#include <media/media_compatibility_layer.h>

#include <QAbstractVideoBuffer>
#include <QAbstractVideoSurface>
#include <QDebug>
#include <QTimer>
#include <QUrl>
#include <QVideoSurfaceFormat>

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
        return (uchar*)0;
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
   : QVideoRendererControl(parent)
   , m_surface(0),
     m_service(service),
     m_textureBuffer(0),
     m_textureId(0),
     m_height(720),
     m_width(1280)
{
    QTimer::singleShot(1, this, SLOT(getTextureId())); // delay until mainloop is running (GL context exists)
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
    Q_UNUSED(context);
    QMetaObject::invokeMethod(AalMediaPlayerService::instance()->videoOutputControl(),
                              "updateVideoTexture", Qt::QueuedConnection);
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
    if (!m_textureBuffer)
        return;

    MediaPlayerWrapper *mp = m_service->androidControl();
    m_service->setVideoSizeCb(AalVideoRendererControl::setVideoSizeCb, static_cast<void *>(this));
    m_service->setVideoTextureNeedsUpdateCb(AalVideoRendererControl::updateVideoTextureCb, static_cast<void *>(this));
    android_media_set_preview_texture(mp, m_textureBuffer->handle().toUInt());
}

void AalVideoRendererControl::getTextureId()
{
    glGenTextures(1, &m_textureId);
    if (m_textureId == 0) {
        qWarning() << "unanble to get texture ID";
        return;
    }

    m_textureBuffer = new AalGLTextureBuffer(m_textureId);
    setupSurface();
}

void AalVideoRendererControl::setVideoSize(int height, int width)
{
    m_height = height;
    m_width = width;

    qDebug() << "Height: " << height << " Width: " << width << endl;
}

void AalVideoRendererControl::updateVideoTexture()
{
    if (!m_surface || !m_textureBuffer)
    {
        qWarning() << "m_surface or m_textureBuffer is NULL, can't update video texture" << endl;
        return;
    }

    if (m_textureId == 0)
    {
        qWarning() << "m_textureId == 0, can't update video texture" << endl;
        return;
    }

    QVideoFrame frame(new AalGLTextureBuffer(m_textureId), QSize(m_width, m_height), QVideoFrame::Format_RGB32);
    //static QVideoFrame frame(m_textureBuffer, QSize(m_width, m_height), QVideoFrame::Format_RGB32);
    //if (m_frame == NULL)
    //    m_frame = new QVideoFrame(m_textureBuffer, QSize(m_width, m_height), QVideoFrame::Format_RGB32);

    if (!frame.isValid())
        return;

    MediaPlayerWrapper *mp = m_service->androidControl();
    frame.setMetaData("MediaPlayerControl", (int)mp);

    if (!m_surface->isActive()) {
        QVideoSurfaceFormat format(frame.size(), frame.pixelFormat(), frame.handleType());

        if (!m_surface->start(format)) {
            qWarning() << "Failed to start video surface with format:" << format;
        }
    }

    if (m_surface->isActive()) {
        m_surface->present(frame);
    }
}
