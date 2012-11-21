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

#include "aalmediaplayercontrol.h"
#include "aalmediaplayerservice.h"
#include "aalvideorenderercontrol.h"
#if 0
#include "aalcameraflashcontrol.h"
#include "aalcamerafocuscontrol.h"
#include "aalcameraservice.h"
#include "aalcamerazoomcontrol.h"
#include "aalimagecapturecontrol.h"
#endif

#include <cassert>
#include <errno.h>

#include <QDebug>

enum {
    OK          = 0,
    NO_ERROR    = 0,
    BAD_VALUE   = -EINVAL,
};

void error_msg_cb(void* context)
{
    printf("%s \n", __PRETTY_FUNCTION__);
}

AalMediaPlayerService *AalMediaPlayerService::m_service = 0;

AalMediaPlayerService::AalMediaPlayerService(QObject *parent):
    QMediaService(parent),
    m_androidMediaPlayer(0)
{
    m_service = this;

    //m_cameraListener = new CameraControlListener;
    //memset(m_cameraListener, 0, sizeof(*m_cameraListener));

    m_videoOutput = new AalVideoRendererControl(this);
    m_mediaPlayerControl = new AalMediaPlayerControl(this);

#if 0
    m_flashControl = new AalCameraFlashControl(this);
    m_focusControl = new AalCameraFocusControl(this);
    m_zoomControl = new AalCameraZoomControl(this);
    m_imageCaptureControl = new AalImageCaptureControl(this);

    m_cameraListener->on_msg_error_cb = error_msg_cb;
#endif
}

AalMediaPlayerService::~AalMediaPlayerService()
{
    m_mediaPlayerControl->stop();
    delete m_mediaPlayerControl;

    delete m_videoOutput;
#if 0
    delete m_flashControl;
    delete m_focusControl;
    delete m_zoomControl;
    delete m_imageCaptureControl;
#endif
    delete m_androidMediaPlayer;
}

QMediaControl *AalMediaPlayerService::requestControl(const char *name)
{
    if (qstrcmp(name, QMediaPlayerControl_iid) == 0)
        return m_mediaPlayerControl;

    if (qstrcmp(name, QVideoRendererControl_iid) == 0)
        return m_videoOutput;

#if 0
    if (qstrcmp(name, QCameraFlashControl_iid) == 0)
        return m_flashControl;

    if (qstrcmp(name, QCameraFocusControl_iid) == 0)
        return m_focusControl;

    if (qstrcmp(name, QCameraImageCaptureControl_iid) == 0)
        return m_imageCaptureControl;

    if (qstrcmp(name, QCameraZoomControl_iid) == 0)
        return m_zoomControl;

#endif

    return 0;
}

void AalMediaPlayerService::releaseControl(QMediaControl *control)
{
    Q_UNUSED(control);
}

MediaPlayerWrapper *AalMediaPlayerService::androidControl()
{
    return m_androidMediaPlayer;
}

bool AalMediaPlayerService::newMediaPlayer()
{
    if (m_androidMediaPlayer)
        return true;

    m_androidMediaPlayer = android_media_new_player();
    if (!m_androidMediaPlayer) {
        qWarning() << "Unable to create a new media player instance.";
        return false;
    }

    m_videoOutput->setupSurface();

#if 0
    m_cameraListener->context = m_androidMediaPlayer;
    m_imageCaptureControl->init(m_androidMediaPlayer);
    m_flashControl->init(m_androidMediaPlayer);
    m_focusControl->init(m_androidMediaPlayer, m_cameraListener);
    m_zoomControl->init(m_androidMediaPlayer, m_cameraListener);
#endif

    return true;
}

void AalMediaPlayerService::setMedia(const QUrl &url)
{
    if (url.isEmpty())
    {
        qWarning() << "Failed to set media source, url must be set." << endl;
        return;
    }

    int ret = android_media_set_data_source(m_androidMediaPlayer, url.path().toStdString().c_str());
    if (ret != OK)
    {
        qWarning() << "Failed to set media source." << endl;
        return;
    }
}

void AalMediaPlayerService::play()
{
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_play(m_androidMediaPlayer);
    if (ret != OK)
    {
        qWarning() << "Failed to play media." << endl;
        return;
    }
}

void AalMediaPlayerService::pause()
{
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_pause(m_androidMediaPlayer);
    if (ret != OK)
    {
        qWarning() << "Failed to pause media playback." << endl;
        return;
    }
}

void AalMediaPlayerService::stop()
{
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_stop(m_androidMediaPlayer);
    if (ret != OK)
    {
        qWarning() << "Failed to stop media playback." << endl;
        return;
    }
}

int AalMediaPlayerService::position()
{
    assert(m_androidMediaPlayer != NULL);

    int pos_msec = 0;
    int ret = android_media_get_current_position(m_androidMediaPlayer, &pos_msec);
    if (ret != OK)
    {
        qWarning() << "Failed to get the current playback position." << endl;
    }

    // qDebug() << "pos_msec: " << pos_msec << endl;

    return pos_msec;
}

void AalMediaPlayerService::setPosition(int msec)
{
    assert(m_androidMediaPlayer != NULL);

    qDebug() << "Seeking to position (msec): " << msec << endl;
    int ret = android_media_seek_to(m_androidMediaPlayer, msec);
    if (ret != OK)
    {
        qWarning() << "Failed to set the current playback position." << endl;
        return;
    }
}

int AalMediaPlayerService::duration()
{
    assert(m_androidMediaPlayer != NULL);

    int duration_msec = 0;
    int ret = android_media_get_duration(m_androidMediaPlayer, &duration_msec);
    if (ret != OK)
    {
        qWarning() << "Failed to get the media duration." << endl;
    }

    qDebug() << "duration_msec: " << duration_msec << endl;

    return duration_msec;
}

void AalMediaPlayerService::setVideoTextureNeedsUpdateCb(on_video_texture_needs_update cb, void *context)
{
    assert(m_androidMediaPlayer != NULL);

    android_media_set_video_texture_needs_update_cb(m_androidMediaPlayer, cb, context);
}

void AalMediaPlayerService::setVideoSizeCb(on_msg_set_video_size cb, void *context)
{
    assert(m_androidMediaPlayer != NULL);

    android_media_set_video_size_cb(m_androidMediaPlayer, cb, context);
}
