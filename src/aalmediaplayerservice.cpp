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

#include <cassert>
#include <errno.h>

#include <QDebug>

enum {
    OK          = 0,
    NO_ERROR    = 0,
    BAD_VALUE   = -EINVAL,
};

static void error_msg_cb(void *context)
{
    Q_UNUSED(context);
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

AalMediaPlayerService *AalMediaPlayerService::m_service = 0;

AalMediaPlayerService::AalMediaPlayerService(QObject *parent):
    QMediaService(parent),
    m_androidMediaPlayer(NULL),
    m_mediaPlayerControlRef(0),
    m_videoOutputRef(0),
    m_setVideoSizeCb(0),
    m_setVideoSizeContext(0)
{
    m_service = this;

    m_videoOutput = new AalVideoRendererControl(this);
    m_mediaPlayerControl = new AalMediaPlayerControl(this);
}

AalMediaPlayerService::~AalMediaPlayerService()
{
    if (m_mediaPlayerControl != NULL)
        delete m_mediaPlayerControl;
    if (m_videoOutput != NULL)
        delete m_videoOutput;
    if (m_androidMediaPlayer != NULL)
        delete m_androidMediaPlayer;
}

QMediaControl *AalMediaPlayerService::requestControl(const char *name)
{
    if (qstrcmp(name, QMediaPlayerControl_iid) == 0)
    {
        if (m_mediaPlayerControlRef == 0 && m_mediaPlayerControl == NULL)
            m_mediaPlayerControl = new AalMediaPlayerControl(this);

        ++m_mediaPlayerControlRef;
        return m_mediaPlayerControl;
    }

    if (qstrcmp(name, QVideoRendererControl_iid) == 0)
    {
        if (m_videoOutputRef == 0 && m_videoOutput == NULL)
            m_videoOutput = new AalVideoRendererControl(this);

        ++m_videoOutputRef;
        return m_videoOutput;
    }

    return NULL;
}

void AalMediaPlayerService::releaseControl(QMediaControl *control)
{
    if (control == m_mediaPlayerControl)
    {
        if (m_mediaPlayerControlRef > 0)
            --m_mediaPlayerControlRef;

        if (m_mediaPlayerControlRef == 0)
        {
            if (m_mediaPlayerControl != NULL)
            {
                delete m_mediaPlayerControl;
                m_mediaPlayerControl = NULL;
                control = NULL;
            }
        }
    }
    else if (control == m_videoOutput)
    {
        if (m_videoOutputRef > 0)
            --m_videoOutputRef;

        if (m_videoOutputRef == 0)
        {
            if (m_videoOutput != NULL)
            {
                delete m_videoOutput;
                m_videoOutput = NULL;
                control = NULL;
            }
        }
    }
}

MediaPlayerWrapper *AalMediaPlayerService::androidControl()
{
    return m_androidMediaPlayer;
}

bool AalMediaPlayerService::newMediaPlayer()
{
    assert(m_videoOutput != NULL);

    if (m_androidMediaPlayer)
        return true;

    m_androidMediaPlayer = android_media_new_player();
    if (!m_androidMediaPlayer) {
        qWarning() << "Unable to create a new media player instance.";
        return false;
    }

    assert(m_setVideoSizeCb != NULL);
    android_media_set_video_size_cb(m_androidMediaPlayer, m_setVideoSizeCb, m_setVideoSizeContext);

    m_videoOutput->setupSurface();
    // Gets called when there is any type of media playback issue
    android_media_set_error_cb(m_androidMediaPlayer, error_msg_cb, static_cast<void *>(this));

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

int AalMediaPlayerService::position() const
{
    assert(m_androidMediaPlayer != NULL);

    int pos_msec = 0;
    int ret = android_media_get_current_position(m_androidMediaPlayer, &pos_msec);
    if (ret != OK)
    {
        qWarning() << "Failed to get the current playback position." << endl;
    }

    return pos_msec;
}

void AalMediaPlayerService::setPosition(int msec)
{
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_seek_to(m_androidMediaPlayer, msec);
    if (ret != OK)
    {
        qWarning() << "Failed to set the current playback position." << endl;
        return;
    }
}

int AalMediaPlayerService::duration() const
{
    assert(m_androidMediaPlayer != NULL);

    int duration_msec = 0;
    int ret = android_media_get_duration(m_androidMediaPlayer, &duration_msec);
    if (ret != OK)
    {
        qWarning() << "Failed to get the media duration." << endl;
    }

    return duration_msec;
}

int AalMediaPlayerService::getVolume() const
{
    assert(m_androidMediaPlayer != NULL);

    int vol = 0;
    int ret = android_media_get_volume(m_androidMediaPlayer, &vol);
    if (ret != OK)
    {
        qWarning() << "Failed to get the volume." << endl;
    }

    return vol;
}

void AalMediaPlayerService::setVolume(int volume)
{
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_set_volume(m_androidMediaPlayer, volume);
    if (ret != OK)
    {
        qWarning() << "Failed to set the volume." << endl;
    }
}

void AalMediaPlayerService::setVideoTextureNeedsUpdateCb(on_video_texture_needs_update cb, void *context)
{
    assert(m_androidMediaPlayer != NULL);

    android_media_set_video_texture_needs_update_cb(m_androidMediaPlayer, cb, context);
}

void AalMediaPlayerService::setVideoSizeCb(on_msg_set_video_size cb, void *context)
{
    m_setVideoSizeCb = cb;
    m_setVideoSizeContext = context;
}

void AalMediaPlayerService::setPlaybackCompleteCb(on_playback_complete cb, void *context)
{
    assert(m_androidMediaPlayer != NULL);

    android_media_set_playback_complete_cb(m_androidMediaPlayer, cb, context);
}
