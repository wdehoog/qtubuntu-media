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

#ifndef AALMEDIAPLAYERSERVICE_H
#define AALMEDIAPLAYERSERVICE_H

#include "media_compatibility_layer.h"

#include <QMediaService>

class AalMediaPlayerControl;
class QMediaPlayerControl;
class AalVideoRendererControl;

struct MediaPlayerWrapper;
//struct CameraControlListener;

class AalMediaPlayerService : public QMediaService
{
Q_OBJECT
public:
    AalMediaPlayerService(QObject *parent = 0);
    ~AalMediaPlayerService();

    QMediaControl* requestControl(const char *name);
    void releaseControl(QMediaControl *control);

    AalMediaPlayerControl *mediaPlayerControl() const { return m_mediaPlayerControl; }
    AalVideoRendererControl *videoOutputControl() const { return m_videoOutput; }
#if 0
    AalCameraFlashControl *flashControl() const { return m_flashControl; }
    AalCameraFocusControl *focusControl() const { return m_focusControl; }
    AalCameraZoomControl *zoomControl() const { return m_zoomControl; }
    AalImageCaptureControl *imageCaptureControl() const { return m_imageCaptureControl; }
#endif

    MediaPlayerWrapper *androidControl();

    //CameraControlListener *listener() {return m_cameraListener; }

    // Was connectCamera:
    bool newMediaPlayer();

    void setMedia(const QUrl &url);
    void play();
    void pause();
    void stop();
    int position();
    void setPosition(int msec);
    int duration();

    void setVideoTextureNeedsUpdateCb(on_video_texture_needs_update cb, void *context);
    void setVideoSizeCb(on_msg_set_video_size cb, void *context);

    static AalMediaPlayerService *instance() { return m_service; }

private:
    static AalMediaPlayerService *m_service;

    AalMediaPlayerControl *m_mediaPlayerControl;
    AalVideoRendererControl *m_videoOutput;
#if 0
    AalCameraFlashControl *m_flashControl;
    AalCameraFocusControl *m_focusControl;
    AalCameraZoomControl *m_zoomControl;
    AalImageCaptureControl *m_imageCaptureControl;
#endif

    MediaPlayerWrapper *m_androidMediaPlayer;
    //CameraControlListener *m_cameraListener;

};

#endif
