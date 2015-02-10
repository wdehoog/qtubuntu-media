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
#include "aalmetadatareadercontrol.h"

#include <core/media/service.h>
#include <core/media/track_list.h>

#include <errno.h>

#include <QAbstractVideoSurface>
#include <QTimerEvent>
#include <QThread>

#include <qtubuntu_media_signals.h>

// Defined in aalvideorenderercontrol.h
#ifdef MEASURE_PERFORMANCE
#include <QDateTime>
#endif

#include <QDebug>

namespace media = core::ubuntu::media;

using namespace std::placeholders;

namespace
{
enum {
    OK          = 0,
    NO_ERROR    = 0,
    BAD_VALUE   = -EINVAL,
};

core::ubuntu::media::Player::FrameAvailableCb empty_frame_available_cb = [](void*)
{
};

core::ubuntu::media::Player::PlaybackCompleteCb empty_playback_complete_cb = [](void*)
{
};

core::Signal<void> the_void;
}

AalMediaPlayerService::AalMediaPlayerService(QObject *parent):
    QMediaService(parent),
    m_hubPlayerSession(NULL),
    m_playbackStatusChangedConnection(the_void.connect([](){})),
    m_errorConnection(the_void.connect([](){})),
    m_mediaPlayerControl(nullptr),
    m_videoOutput(nullptr),
    m_metaDataReaderControl(nullptr),
    m_videoOutputReady(false),
    m_cachedDuration(0),
    m_mediaPlaylist(NULL)
#ifdef MEASURE_PERFORMANCE
     , m_lastFrameDecodeStart(0)
     , m_currentFrameDecodeStart(0)
     , m_avgCount(0)
     , m_frameDecodeAvg(0)
#endif
{
    qDebug() << "Here1";
    m_hubService = media::Service::Client::instance();
    qDebug() << "m_hubService (nullptr?): " << (m_hubService == nullptr);
    qDebug() << "Construct (this: " << this << ")";
    qDebug() << "Here2";

    if (!newMediaPlayer())
        qWarning() << "Failed to create a new media player backend. Video playback will not function." << endl;
    qDebug() << "Here3";
    qDebug() << "m_hubService (after newMediaPlayer): " << (m_hubService == nullptr);

    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Could not finish contructing new AalMediaPlayerService instance since m_hubPlayerSession is NULL";
        return;
    }

    qDebug() << "Creating new QMediaPlayerControl instance";
    createMediaPlayerControl();
    qDebug() << "Creating new QVideoRendererControl instance";
    createVideoRendererControl();
    qDebug() << "Creating new QMetaDataReaderControl instance";
    createMetaDataReaderControl();

    m_playbackStatusChangedConnection = m_hubPlayerSession->playback_status_changed().connect(
            std::bind(&AalMediaPlayerService::onPlaybackStatusChanged, this, _1));
    m_errorConnection = m_hubPlayerSession->error().connect(
            std::bind(&AalMediaPlayerService::onError, this, _1));
}

AalMediaPlayerService::AalMediaPlayerService(const std::shared_ptr<core::ubuntu::media::Service> &service,
                                             const std::shared_ptr<core::ubuntu::media::Player> &player, QObject *parent)
   : QMediaService(parent),
    m_hubService(service),
    m_hubPlayerSession(player),
    m_playbackStatusChangedConnection(the_void.connect([](){})),
    m_errorConnection(the_void.connect([](){})),
    m_mediaPlayerControl(nullptr),
    m_videoOutput(nullptr),
    m_metaDataReaderControl(nullptr),
    m_videoOutputReady(false),
    m_cachedDuration(0),
    m_mediaPlaylist(NULL)
#ifdef MEASURE_PERFORMANCE
     , m_lastFrameDecodeStart(0)
     , m_currentFrameDecodeStart(0)
     , m_avgCount(0)
     , m_frameDecodeAvg(0)
#endif
{
    qDebug() << "Here0";
    qDebug() << "m_hubService (nullptr?): " << (m_hubService == nullptr);
    qDebug() << "Construct (this: " << this << ")";
    qDebug() << "Here1";

    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Could not finish contructing new AalMediaPlayerService instance since m_hubPlayerSession is NULL";
        return;
    }

    qDebug() << "Creating new QMediaPlayerControl instance";
    createMediaPlayerControl();
    qDebug() << "Creating new QVideoRendererControl instance";
    createVideoRendererControl();
    qDebug() << "Creating new QMetaDataReaderControl instance";
    createMetaDataReaderControl();

    m_playbackStatusChangedConnection = m_hubPlayerSession->playback_status_changed().connect(
            std::bind(&AalMediaPlayerService::onPlaybackStatusChanged, this, _1));
    m_errorConnection = m_hubPlayerSession->error().connect(
            std::bind(&AalMediaPlayerService::onError, this, _1));
}

AalMediaPlayerService::~AalMediaPlayerService()
{
    m_errorConnection.disconnect();
    m_playbackStatusChangedConnection.disconnect();

    deleteMediaPlayerControl();
    deleteVideoRendererControl();
    deleteMetaDataReaderControl();
}

QMediaControl *AalMediaPlayerService::requestControl(const char *name)
{
    qDebug() << Q_FUNC_INFO;
    if (qstrcmp(name, QMediaPlayerControl_iid) == 0)
    {
        qDebug() << "Requested QMediaPlayerControl";
        if (not m_mediaPlayerControl)
            createMediaPlayerControl();

        return m_mediaPlayerControl;
    }

    if (qstrcmp(name, QVideoRendererControl_iid) == 0)
    {
        qDebug() << "Requested QVideoRendererControl";
        if (not m_videoOutput)
            createVideoRendererControl();

        return m_videoOutput;
    }

    if (qstrcmp(name, QMetaDataReaderControl_iid) == 0)
    {
        qDebug() << "Requested QMetaDataReaderControl: '" << name << "'";
        qDebug() << "Request (this: " << this << ")";
        if (not m_metaDataReaderControl)
            createMetaDataReaderControl();

        return m_metaDataReaderControl;
    }

    return NULL;
}

void AalMediaPlayerService::releaseControl(QMediaControl *control)
{
    if (control == m_mediaPlayerControl)
        deleteMediaPlayerControl();
    else if (control == m_videoOutput)
        deleteVideoRendererControl();
    else if (control == m_metaDataReaderControl)
        deleteMetaDataReaderControl();
    else
        delete control;
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

        // This lambda gets called after every successfully decoded video frame
        m_hubPlayerSession->set_frame_available_callback([](void *context)
        {
            if (context != NULL)
            {
                auto s = static_cast<AalMediaPlayerService*>(context);

                // We might receive this callback after the control has been released.
                // In that case, we return early.
                if (not s->videoOutputControl())
                    return;

#ifdef MEASURE_PERFORMANCE
                s->measurePerformance();
#endif
                QMetaObject::invokeMethod(s->videoOutputControl(), "updateVideoTexture", Qt::QueuedConnection);
            }
        },
        static_cast<void*>(this));
    }
    catch (std::runtime_error &e) {
        qWarning() << "Failed to create video sink: " << e.what();
        return;
    }

    m_videoOutputReady = true;
}

QMediaPlayer::AudioRole AalMediaPlayerService::audioRole() const
{
    if (m_hubPlayerSession == NULL)
        return QMediaPlayer::MultimediaRole;

    return static_cast<QMediaPlayer::AudioRole>(m_hubPlayerSession->audio_stream_role().get());
}

void AalMediaPlayerService::setAudioRole(QMediaPlayer::AudioRole audioRole)
{
    if (m_hubPlayerSession == NULL)
        return;

    qDebug() << __PRETTY_FUNCTION__;
    m_hubPlayerSession->audio_stream_role().set(static_cast<media::Player::AudioStreamRole>(audioRole));
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
            media::Player::PlayerKey key = m_hubPlayerSession->key();
            // Pause all other music and video sessions
            m_hubService->pause_other_sessions(key);

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

int64_t AalMediaPlayerService::duration()
{
    if (m_hubPlayerSession == NULL)
    {
        qWarning() << "Cannot get playback duration without a valid media-hub player session";
        return 0;
    }

    try {
        const int64_t d = m_hubPlayerSession->duration();
        // Make sure that apps get updated if the duration does in fact change
        if (d != m_cachedDuration)
        {
            m_cachedDuration = d;
            m_mediaPlayerControl->emitDurationChanged(d / 1e6);
        }
        return d / 1e6;
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

void AalMediaPlayerService::createMediaPlayerControl()
{
    if (m_hubPlayerSession == NULL)
        return;

    m_mediaPlayerControl = new AalMediaPlayerControl(this);
    m_hubPlayerSession->set_playback_complete_callback([](void *context)
    {
        auto control = static_cast<AalMediaPlayerControl*>(context);
        control->playbackComplete();
    },
    m_mediaPlayerControl);
}

void AalMediaPlayerService::createVideoRendererControl()
{
    if (m_hubPlayerSession == NULL)
        return;

    m_videoOutput = new AalVideoRendererControl(this);
}

void AalMediaPlayerService::createMetaDataReaderControl()
{
    qDebug() << Q_FUNC_INFO;
//    if (m_hubPlayerSession == NULL)
//        return;

    qDebug() << "Creating QMetaDataReaderControl";
    m_metaDataReaderControl = new AalMetaDataReaderControl(this);

    if (m_mediaPlayerControl == nullptr)
        qDebug() << "m_mediaPlayerControl is NULL";

    connect(m_mediaPlayerControl, SIGNAL(mediaChanged(QMediaContent)),
            m_metaDataReaderControl, SLOT(onMediaChanged(QMediaContent)));
#if 0
    connect(m_mediaPlayerControl, SIGNAL(metaDataUpdated()),
            m_metaDataReaderControl, SLOT(onUpdateMetaData()));
#endif
}

void AalMediaPlayerService::deleteMediaPlayerControl()
{
    if (m_hubPlayerSession == NULL)
        return;

    m_hubPlayerSession->set_playback_complete_callback(
                empty_playback_complete_cb,
                nullptr);

    delete m_mediaPlayerControl;
    m_mediaPlayerControl = NULL;
}

void AalMediaPlayerService::deleteVideoRendererControl()
{
    if (m_hubPlayerSession == nullptr)
        return;

    m_hubPlayerSession->set_frame_available_callback(
                empty_frame_available_cb,
                nullptr);

    delete m_videoOutput;
    m_videoOutput = nullptr;
}

void AalMediaPlayerService::deleteMetaDataReaderControl()
{
    if (m_hubPlayerSession == nullptr)
        return;

    delete m_metaDataReaderControl;
    m_metaDataReaderControl = nullptr;
}

void AalMediaPlayerService::signalQMediaPlayerError(const media::Player::Error &error)
{
    QMediaPlayer::Error outError = QMediaPlayer::NoError;
    QString outErrorStr;
    switch (error)
    {
        case media::Player::Error::resource_error:
            outError = QMediaPlayer::ResourceError;
            outErrorStr = "A media resource couldn't be resolved.";
            break;
        case media::Player::Error::format_error:
            outError = QMediaPlayer::FormatError;
            outErrorStr = "The media format type is not playable due to a missing codec.";
            break;
        case media::Player::Error::network_error:
            outError = QMediaPlayer::NetworkError;
            outErrorStr = "A network error occurred.";
            break;
        case media::Player::Error::access_denied_error:
            outError = QMediaPlayer::AccessDeniedError;
            outErrorStr = "Insufficient privileges to play that media.";
            break;
        case media::Player::Error::service_missing_error:
            outError = QMediaPlayer::ServiceMissingError;
            outErrorStr = "A valid playback service was not found, playback cannot proceed.";
            break;
        default:
            break;
    }

    if (outError != QMediaPlayer::NoError)
        m_mediaPlayerControl->error(outError, outErrorStr);
}

void AalMediaPlayerService::onPlaybackStatusChanged(const media::Player::PlaybackStatus &status)
{
    // The media player control might have been released prior to this call. For that, we check for
    // null and return early in that case.
    if (m_mediaPlayerControl == nullptr)
        return;

    // If the playback status changes from underneath (e.g. GStreamer or media-hub), make sure
    // the app is notified about this so it can change it's status
    switch (status)
    {
        case media::Player::PlaybackStatus::ready:
            break;
        case media::Player::PlaybackStatus::stopped:
            // FIXME: Disabled for now since this causes next/previous to not work in music-app
            //m_mediaPlayerControl->setState(QMediaPlayer::StoppedState);
            break;
        case media::Player::PlaybackStatus::paused:
            m_mediaPlayerControl->setState(QMediaPlayer::PausedState);
            break;
        case media::Player::PlaybackStatus::playing:
            m_mediaPlayerControl->setState(QMediaPlayer::PlayingState);
            break;
        default:
            qWarning() << "Unknown PlaybackStatus: " << status;
    }
}

void AalMediaPlayerService::onError(const core::ubuntu::media::Player::Error &error)
{
    qWarning() << "** Media playback error: " << error;
    signalQMediaPlayerError(error);
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

    createMediaPlayerControl();
    createVideoRendererControl();
    createMetaDataReaderControl();

    m_hubPlayerSession->playback_status_changed().connect(
            std::bind(&AalMediaPlayerService::onPlaybackStatusChanged, this, _1));
}

void AalMediaPlayerService::setService(const std::shared_ptr<core::ubuntu::media::Service> &service)
{
    m_hubService = service;
}

#ifdef MEASURE_PERFORMANCE
void AalMediaPlayerService::measurePerformance()
{
    m_currentFrameDecodeStart = QDateTime::currentMSecsSinceEpoch();
    const qint64 delta = m_currentFrameDecodeStart - m_lastFrameDecodeStart;
    if (m_currentFrameDecodeStart != m_lastFrameDecodeStart) {
        m_lastFrameDecodeStart = QDateTime::currentMSecsSinceEpoch();
        if (delta > 0) {
            m_frameDecodeAvg += delta;
            qDebug() << "Frame-to-frame decode delta (ms): " << delta;
        }
    }

    if (m_avgCount == 30) {
        // Ideally if playing a video that was recorded at 30 fps, the average for
        // playback should be close to 30 fps too
        qDebug() << "Frame-to-frame decode average (ms) (30 frames times counted): " << (m_frameDecodeAvg / 30);
        m_avgCount = m_frameDecodeAvg = 0;
    }
    else
        ++m_avgCount;
}
#endif
