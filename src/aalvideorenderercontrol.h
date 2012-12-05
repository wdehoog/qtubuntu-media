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

#ifndef AALVIDEORENDERERCONTROL_H
#define AALVIDEORENDERERCONTROL_H

#include <QImage>
#include <QVideoRendererControl>

#include <qgl.h>

class AalMediaPlayerService;
class AalGLTextureBuffer;

class AalVideoRendererControl : public QVideoRendererControl
{
Q_OBJECT
public:
    AalVideoRendererControl(AalMediaPlayerService *service, QObject *parent = 0);
    ~AalVideoRendererControl();

    QAbstractVideoSurface *surface() const;
    void setSurface(QAbstractVideoSurface *surface);

    // Callbacks
    static void updateVideoTextureCb(void *context);
    static void setVideoSizeCb(int height, int width, void *data);

public Q_SLOTS:
    void setupSurface();

Q_SIGNALS:
    void surfaceChanged(QAbstractVideoSurface *surface);

private Q_SLOTS:
    void getTextureId();
    void setVideoSize(int height, int width);
    void updateVideoTexture();

private:
    QAbstractVideoSurface *m_surface;
    AalMediaPlayerService *m_service;
    AalGLTextureBuffer *m_textureBuffer;
    GLuint m_textureId;

    int m_height;
    int m_width;
};

#endif
