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
class AalAudioRoleControl;
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
            QObject *parent = 0);
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

    QAudio::Role audioRole() const;
    void setAudioRole(QAudio::Role audioRole);

    void setMedia(const QUrl &url);
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

    int bufferStatus() { return m_bufferPercent; }

Q_SIGNALS:
    void serviceReady();
    void playbackComplete();
    void playbackStatusChanged(const core::ubuntu::media::Player::PlaybackStatus &status);

public Q_SLOTS:
    void onPlaybackStatusChanged();
    void onApplicationStateChanged(Qt::ApplicationState state);
    void onServiceDisconnected();
    void onServiceReconnected();
    void onBufferingChanged();

protected:
    void constructNewPlayerService();
    void updateClientSignals();
    void connectSignals();
    void disconnectSignals();
#ifdef MEASURE_PERFORMANCE
    void measurePerformance();
#endif
    void checkForSessionDetach();
    void checkForSessionReattach();

private:
    void createMediaPlayerControl();
    void createVideoRendererControl();
    void createPlaylistControl();
    void createAudioRoleControl();

    void deleteMediaPlayerControl();
    void destroyPlayerSession();
    void deleteVideoRendererControl();
    void deletePlaylistControl();
    void deleteAudioRoleControl();

    // Signals the proper QMediaPlayer::Error from a core::ubuntu::media::Error
    void signalQMediaPlayerError(const core::ubuntu::media::Player::Error &error);
    void onError(const core::ubuntu::media::Player::Error &error);

    inline QString playbackStatusStr(const core::ubuntu::media::Player::PlaybackStatus &status);

    std::shared_ptr<core::ubuntu::media::Service> m_hubService;
    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayerSession;
    core::Connection m_playbackStatusChangedConnection;
    core::Connection m_errorConnection;
    core::Connection m_endOfStreamConnection;
    core::Connection m_serviceDisconnectedConnection;
    core::Connection m_serviceReconnectedConnection;
    core::Connection m_bufferingStatusChangedConnection;

    AalMediaPlayerControl *m_mediaPlayerControl;
    AalVideoRendererControl *m_videoOutput;
    AalMediaPlaylistControl *m_mediaPlaylistControl;
    AalMediaPlaylistProvider *m_mediaPlaylistProvider;
    AalAudioRoleControl *m_audioRoleControl;
    bool m_videoOutputReady;
    bool m_firstPlayback;

    int64_t m_cachedDuration;

    const QMediaPlaylist* m_mediaPlaylist;

    core::ubuntu::media::Player::PlaybackStatus m_newStatus;
    int m_bufferPercent;

    std::string m_sessionUuid;
    bool m_doReattachSession;
    bool m_sessionAttached;

#ifdef MEASURE_PERFORMANCE
    qint64 m_lastFrameDecodeStart;
    qint64 m_currentFrameDecodeStart;
    qint16 m_avgCount;
    qint64 m_frameDecodeAvg;
#endif
};

#endif
