/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
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

#include <core/media/service.h>
#include <core/media/player.h>
#include <core/media/track_list.h>

#include <errno.h>

#include <QAbstractVideoSurface>

#include <QDebug>

namespace media = core::ubuntu::media;

enum {
    OK          = 0,
    NO_ERROR    = 0,
    BAD_VALUE   = -EINVAL,
};

AalMediaPlayerService *AalMediaPlayerService::m_service = 0;

AalMediaPlayerService::AalMediaPlayerService(QObject *parent):
    QMediaService(parent),
    m_hubPlayerSession(NULL),
    m_videoOutputReady(false),
    m_mediaPlayerControlRef(0),
    m_videoOutputRef(0),
    m_mediaPlaylist(NULL)
{
    m_service = this;

    m_hubService = media::Service::Client::instance();

    if (!newMediaPlayer())
        qWarning() << "Failed to create a new media player backend. Video playback will not function." << endl;

    m_videoOutput = new AalVideoRendererControl(this);
    m_mediaPlayerControl = new AalMediaPlayerControl(this);

    if (m_hubPlayerSession == NULL)
        return;

    m_hubPlayerSession->set_playback_complete_callback([](void *context)
    {
        auto control = static_cast<AalMediaPlayerControl*>(context);
        control->playbackComplete();
    },
    static_cast<void*>(m_mediaPlayerControl));
}

AalMediaPlayerService::~AalMediaPlayerService()
{
    if (m_mediaPlayerControl != NULL)
        delete m_mediaPlayerControl;
    if (m_videoOutput != NULL)
        delete m_videoOutput;
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

AalMediaPlayerService::GLConsumerWrapperHybris AalMediaPlayerService::glConsumer() const
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot return glConsumer without a valid media-hub player session";
        return NULL;
    }
    return m_hubPlayerSession->gl_consumer();
}

bool AalMediaPlayerService::newMediaPlayer()
{
    // Only one player session needed
    if (m_hubPlayerSession != NULL)
        return true;

    if (m_hubService == NULL)
    {
        qWarning() << "Cannot create new media player instance without a valid media-hub service instance";
        return false;
    }

    try {
        m_hubPlayerSession = m_hubService->create_session(media::Player::Client::default_configuration());
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to start a new media-hub player session: " << e.what();
        return false;
    }

    return true;
}

void AalMediaPlayerService::createVideoSink(uint32_t texture_id)
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot create a video sink without a valid media-hub player session";
        return;
    }

    try {
        m_hubPlayerSession->create_video_sink(texture_id);
        // This call will make sure the GLConsumerWrapperHybris gets set on qtvideo-node
        m_videoOutput->updateVideoTexture();

        m_hubPlayerSession->set_frame_available_callback(&AalMediaPlayerService::onFrameAvailableCb, static_cast<void*>(this));
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to create video sink: " << e.what();
        return;
    }

    m_videoOutputReady = true;
}

void AalMediaPlayerService::setMediaPlaylist(const QMediaPlaylist &playlist)
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot set playlist without a valid media-hub player session";
        return;
    }
    if (playlist.mediaCount() == 0)
    {
        qWarning() << "Failed to set background playlist, list is empty." << endl;
        return;
    }

    m_mediaPlaylist = &playlist;
}

void AalMediaPlayerService::setMedia(const QUrl &url)
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot open uri without a valid media-hub player session";
        return;
    }
    if (url.isEmpty())
    {
        qWarning() << "Failed to set media source, url must be set." << endl;
        return;
    }

    qDebug() << "Setting media to: " << url;
    const media::Track::UriType uri(url.url().toStdString());
    try {
        m_hubPlayerSession->open_uri(uri);
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to open media " << url << ": " << e.what();
        return;
    }

    m_videoOutput->setupSurface();
}

void AalMediaPlayerService::play()
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot start playback without a valid media-hub player session";
        return;
    }

    // If we previously played and hit the end-of-stream, stop will be called which
    // tears down the video sink. We need a new video sink in order to render video again
    if (!m_videoOutputReady && m_videoOutput->textureId() > 0)
        createVideoSink(m_videoOutput->textureId());

    if ((m_videoOutputReady && isVideoSource())
            || isAudioSource())
    {
        try {
            qDebug() << "Actually calling m_hubPlayerSession->play()";
            m_hubPlayerSession->play();
            m_mediaPlayerControl->mediaPrepared();
        }
        catch (std::runtime_error &e) {
            qWarning() << "Failed to start playback: " << e.what();
            return;
        }
    }
    else
        Q_EMIT serviceReady();
}

void AalMediaPlayerService::pause()
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot pause playback without a valid media-hub player session";
        return;
    }

    try {
        m_hubPlayerSession->pause();
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to pause playback: " << e.what();
        return;
    }
}

void AalMediaPlayerService::stop()
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot stop playback without a valid media-hub player session";
        return;
    }

    try {
        m_hubPlayerSession->stop();
        m_videoOutputReady = false;
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to stop playback: " << e.what();
        return;
    }
}

int64_t AalMediaPlayerService::position() const
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot get current playback position without a valid media-hub player session";
        return 0;
    }

    try {
        return m_hubPlayerSession->position() / 1e6;
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to get current playback position: " << e.what();
        return 0;
    }
}

void AalMediaPlayerService::setPosition(int64_t msec)
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot set current playback position without a valid media-hub player session";
        return;
    }
    m_hubPlayerSession->seek_to(std::chrono::microseconds{msec * 1000});
}

int64_t AalMediaPlayerService::duration() const
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot get playback duration without a valid media-hub player session";
        return 0;
    }

    try {
        return m_hubPlayerSession->duration() / 1e6;
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to get current playback duration: " << e.what();
        return 0;
    }
}

bool AalMediaPlayerService::isVideoSource() const
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot set current playback position without a valid media-hub player session";
        return false;
    }

    try {
        return m_hubPlayerSession->is_video_source();
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to check if source is video: " << e.what();
        return false;
    }
}

bool AalMediaPlayerService::isAudioSource() const
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot set current playback position without a valid media-hub player session";
        return false;
    }

    try {
        return m_hubPlayerSession->is_audio_source();
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to check if source is video: " << e.what();
        return false;
    }
}

int AalMediaPlayerService::getVolume() const
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot get volume without a valid media-hub player session";
        return 0;
    }

    try {
        return m_hubPlayerSession->volume();
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to get current volume level: " << e.what();
        return 0;
    }
}

void AalMediaPlayerService::setVolume(int volume)
{
    Q_UNUSED(volume);

    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot set volume without a valid media-hub player session";
        return;
    }
}

void AalMediaPlayerService::onFrameAvailableCb(void *context)
{
    if (context != NULL)
    {
        AalMediaPlayerService *s = static_cast<AalMediaPlayerService*>(context);
        s->onFrameAvailable();
    }
}

void AalMediaPlayerService::onFrameAvailable()
{
    m_videoOutput->updateVideoTexture();
}

void AalMediaPlayerService::pushPlaylist()
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot push playlist without a valid media-hub player session";
        return;
    }

    if (m_mediaPlaylist == NULL)
        return;

    for (int i = 0; i < m_mediaPlaylist->mediaCount(); i++)
    {
        const media::Track::UriType uri(m_mediaPlaylist->media(i).canonicalUrl().url().toStdString());
        m_hubPlayerSession->track_list()->add_track_with_uri_at(uri, media::TrackList::after_empty_track(), false);
    }
}

void AalMediaPlayerService::setPlayer(const std::shared_ptr<core::ubuntu::media::Player> &player)
{
    m_hubPlayerSession = player;
}
