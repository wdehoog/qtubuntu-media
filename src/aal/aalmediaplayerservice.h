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

#ifndef AALMEDIAPLAYERSERVICE_H
#define AALMEDIAPLAYERSERVICE_H

#include "aalvideorenderercontrol.h"

#include <core/media/player.h>

#include <core/connection.h>

#include <qmediaplayer.h>
#include <QMediaPlaylist>
#include <QMediaService>

#include <memory>

class AalMediaPlayerControl;
class AalMediaPlaylistControl;
class AalMediaPlaylistProvider;
class QMediaPlayerControl;
class AalVideoRendererControl;
class tst_MediaPlayerPlugin;
class QTimerEvent;

namespace core { namespace ubuntu { namespace media {
namespace video {
    class Sink;
}
class Service;
class Player;
class TrackList;
} } }

class AalMediaPlayerService : public QMediaService
{
    Q_OBJECT

    // For unit testing purposes
    friend class tst_MediaPlayerPlugin;

public:
    typedef void *GLConsumerWrapperHybris;

    AalMediaPlayerService(QObject *parent = 0);
    AalMediaPlayerService(const std::shared_ptr<core::ubuntu::media::Service> &service,
            const std::shared_ptr<core::ubuntu::media::Player> &player, QObject *parent = 0);
    ~AalMediaPlayerService();

    QMediaControl* requestControl(const char *name);
    void releaseControl(QMediaControl *control);

    AalMediaPlayerControl *mediaPlayerControl() const { return m_mediaPlayerControl; }
    AalMediaPlaylistControl *mediaPlaylistControl() const { return m_mediaPlaylistControl; }
    AalVideoRendererControl *videoOutputControl() const { return m_videoOutput; }

    bool newMediaPlayer();

    std::shared_ptr<core::ubuntu::media::video::Sink> createVideoSink(uint32_t texture_id);
    // Call this before attempting to play the same video a second time (after EOS)
    void resetVideoSink();

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    QMediaPlayer::AudioRole audioRole() const;
    void setAudioRole(QMediaPlayer::AudioRole audioRole);
#else
    QAudio::Role audioRole() const;
    void setAudioRole(QAudio::Role audioRole);
#endif

    void setMedia(const QUrl &url);
    void setMedia(const QMediaContent &media);
    void setMediaPlaylist(const QMediaPlaylist& playlist);
    void play();
    void pause();
    void stop();
    int64_t position() const;
    void setPosition(int64_t msec);
    int64_t duration();
    bool isVideoSource() const;
    bool isAudioSource() const;

    int getVolume() const;
    void setVolume(int volume);

    void pushPlaylist();

    const std::shared_ptr<core::ubuntu::media::Player>& getPlayer() const { return m_hubPlayerSession; }

    /* This is for unittest purposes to be able to set a mock-object version of a
     * player object */
    void setPlayer(const std::shared_ptr<core::ubuntu::media::Player> &player);
    /* This is for unittest purposes to be able to set a mock-object version of a
     * service object */
    void setService(const std::shared_ptr<core::ubuntu::media::Service> &service);

Q_SIGNALS:
    void serviceReady();
    void playbackComplete();
    void playbackStatusChanged(const core::ubuntu::media::Player::PlaybackStatus &status);

public Q_SLOTS:
    void onPlaybackStatusChanged();
    void onApplicationStateChanged(Qt::ApplicationState state);

protected:
    void updateClientSignals();
    void connect_signals();
    void disconnect_signals();
#ifdef MEASURE_PERFORMANCE
    void measurePerformance();
#endif

private:
    void createMediaPlayerControl();
    void createVideoRendererControl();
    void createPlaylistControl();

    void deleteMediaPlayerControl();
    void destroyPlayerSession();
    void deleteVideoRendererControl();
    void deletePlaylistControl();

    // Signals the proper QMediaPlayer::Error from a core::ubuntu::media::Error
    void signalQMediaPlayerError(const core::ubuntu::media::Player::Error &error);

    void onError(const core::ubuntu::media::Player::Error &error);

    std::shared_ptr<core::ubuntu::media::Service> m_hubService;
    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayerSession;
    core::Connection m_playbackStatusChangedConnection;
    core::Connection m_errorConnection;
    core::Connection m_endOfStreamConnection;

    AalMediaPlayerControl *m_mediaPlayerControl;
    AalVideoRendererControl *m_videoOutput;
    AalMediaPlaylistControl *m_mediaPlaylistControl;
    AalMediaPlaylistProvider *m_mediaPlaylistProvider;
    bool m_videoOutputReady;
    bool m_firstPlayback;

    int64_t m_cachedDuration;

    const QMediaPlaylist* m_mediaPlaylist;

    core::ubuntu::media::Player::PlaybackStatus m_newStatus;
    std::string m_sessionUuid;
    bool m_doReattachSession;

#ifdef MEASURE_PERFORMANCE
    qint64 m_lastFrameDecodeStart;
    qint64 m_currentFrameDecodeStart;
    qint16 m_avgCount;
    qint64 m_frameDecodeAvg;
#endif
};

#endif
