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

#include "aalmediaplayercontrol.h"
#include "aalmediaplayerservice.h"
#include "aalvideorenderercontrol.h"

#include <cassert>
#include <errno.h>

#include <QDebug>

namespace media = core::ubuntu::media;

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
    m_mediaPlayerControlRef(0),
    m_videoOutputRef(0),
    m_setVideoSizeCb(0),
    m_setVideoSizeContext(0)
{
    m_service = this;

    m_hubService = media::Service::Client::instance();

    if (!newMediaPlayer())
        qWarning() << "Failed to create a new media player backend. Video playback will not function." << endl;

    m_videoOutput = new AalVideoRendererControl(this);
    m_mediaPlayerControl = new AalMediaPlayerControl(this);
}

AalMediaPlayerService::~AalMediaPlayerService()
{
    if (m_mediaPlayerControl != NULL)
        delete m_mediaPlayerControl;
    if (m_videoOutput != NULL)
        delete m_videoOutput;
#if 0
    if (m_androidMediaPlayer != NULL)
        delete m_androidMediaPlayer;
#endif
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
#if 0
    return m_androidMediaPlayer;
#endif
    return NULL;
}

bool AalMediaPlayerService::newMediaPlayer()
{
#if 0
    if (m_androidMediaPlayer)
        return true;

    m_androidMediaPlayer = android_media_new_player();
    if (!m_androidMediaPlayer) {
        qWarning() << "Unable to create a new media player instance.";
        return false;
    }
#endif

    // Only one player session needed
    if (m_hubPlayerSession)
        return true;

    m_hubPlayerSession = m_hubService->create_session(media::Player::Client::default_configuration());

    return true;
}

void AalMediaPlayerService::setupMediaPlayer()
{
#if 0
    assert(m_androidMediaPlayer != NULL);

    assert(m_setVideoSizeCb != NULL);
    android_media_set_video_size_cb(m_androidMediaPlayer, m_setVideoSizeCb, m_setVideoSizeContext);
#endif

    //m_videoOutput->setupSurface();
#if 0
    // Gets called when there is any type of media playback issue
    android_media_set_error_cb(m_androidMediaPlayer, error_msg_cb, static_cast<void *>(this));
#endif
}

void AalMediaPlayerService::createVideoSink(uint32_t texture_id)
{
    assert(m_hubPlayerSession != NULL);

    // TODO
    //m_hubPlayerSession->create_video_sink(texture_id);
}

void AalMediaPlayerService::setMedia(const QUrl &url)
{
    if (url.isEmpty())
    {
        qWarning() << "Failed to set media source, url must be set." << endl;
        return;
    }

    qDebug() << "Setting media to: " << url;
    const media::Track::UriType uri(url.url().toStdString());
    qDebug() << "Got the uri from stdString";
    m_hubPlayerSession->open_uri(uri);

    // TODO
    //m_videoOutput->setupSurface();

#if 0
    int ret = android_media_set_data_source(m_androidMediaPlayer, url.path().toStdString().c_str());
    if (ret != OK)
    {
        qWarning() << "Failed to set media source." << endl;
        return;
    }
#endif
    qDebug() << "Finished setMedia";
}

void AalMediaPlayerService::play()
{
    assert(m_hubPlayerSession != NULL);

    m_hubPlayerSession->play();
#if 0
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_play(m_androidMediaPlayer);
    if (ret != OK)
    {
        qWarning() << "Failed to play media." << endl;
        return;
    }
#endif

    Q_EMIT serviceReady();
}

void AalMediaPlayerService::pause()
{
    assert(m_hubPlayerSession != NULL);

    m_hubPlayerSession->pause();
#if 0
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_pause(m_androidMediaPlayer);
    if (ret != OK)
    {
        qWarning() << "Failed to pause media playback." << endl;
        return;
    }
#endif
}

void AalMediaPlayerService::stop()
{
    assert(m_hubPlayerSession != NULL);

    m_hubPlayerSession->stop();
#if 0
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_stop(m_androidMediaPlayer);
    if (ret != OK)
    {
        qWarning() << "Failed to stop media playback." << endl;
        return;
    }
#endif
}

int AalMediaPlayerService::position() const
{
    assert(m_hubPlayerSession != NULL);
#if 0
    assert(m_androidMediaPlayer != NULL);

    int pos_msec = 0;
    int ret = android_media_get_current_position(m_androidMediaPlayer, &pos_msec);
    if (ret != OK)
    {
        qWarning() << "Failed to get the current playback position." << endl;
    }

    return pos_msec;
#endif
    qDebug() << "m_hubPlayerSession->position(): " << m_hubPlayerSession->position();
    return m_hubPlayerSession->position();
}

void AalMediaPlayerService::setPosition(int msec)
{
    assert(m_hubPlayerSession != NULL);

    m_hubPlayerSession->seek_to(std::chrono::milliseconds{msec * 1000});
#if 0
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_seek_to(m_androidMediaPlayer, msec);
    if (ret != OK)
    {
        qWarning() << "Failed to set the current playback position." << endl;
        return;
    }
#endif
}

int AalMediaPlayerService::duration() const
{
    assert(m_hubPlayerSession != NULL);
#if 0
    assert(m_androidMediaPlayer != NULL);

    int duration_msec = 0;
    int ret = android_media_get_duration(m_androidMediaPlayer, &duration_msec);
    if (ret != OK)
    {
        qWarning() << "Failed to get the media duration." << endl;
    }

    return duration_msec;
#endif
    return 0;
}

int AalMediaPlayerService::getVolume() const
{
    assert(m_hubPlayerSession != NULL);
#if 0
    assert(m_androidMediaPlayer != NULL);

    int vol = 0;
    int ret = android_media_get_volume(m_androidMediaPlayer, &vol);
    if (ret != OK)
    {
        qWarning() << "Failed to get the volume." << endl;
    }

    return vol;
#endif
    //return m_hubPlayerSession->volume();
    return 0;
}

void AalMediaPlayerService::setVolume(int volume)
{
    assert(m_hubPlayerSession != NULL);

#if 0
    assert(m_androidMediaPlayer != NULL);

    int ret = android_media_set_volume(m_androidMediaPlayer, volume);
    if (ret != OK)
    {
        qWarning() << "Failed to set the volume." << endl;
    }
#endif
}

void AalMediaPlayerService::setVideoTextureNeedsUpdateCb(on_video_texture_needs_update cb, void *context)
{
#if 0
    assert(m_androidMediaPlayer != NULL);

    android_media_set_video_texture_needs_update_cb(m_androidMediaPlayer, cb, context);
#endif
}

void AalMediaPlayerService::setVideoSizeCb(on_msg_set_video_size cb, void *context)
{
    m_setVideoSizeCb = cb;
    m_setVideoSizeContext = context;
}

void AalMediaPlayerService::setPlaybackCompleteCb(on_playback_complete cb, void *context)
{
#if 0
    assert(m_androidMediaPlayer != NULL);

    android_media_set_playback_complete_cb(m_androidMediaPlayer, cb, context);
#endif
}

void AalMediaPlayerService::setMediaPreparedCb(on_media_prepared cb, void *context)
{
#if 0
    assert(m_androidMediaPlayer != NULL);

    android_media_set_media_prepared_cb(m_androidMediaPlayer, cb, context);
#endif
}
