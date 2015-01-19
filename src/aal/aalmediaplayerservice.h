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

#include <qmediaplayer.h>
#include <QMediaPlaylist>
#include <QMediaService>

#include <memory>

class AalMediaPlayerControl;
class QMediaPlayerControl;
class AalVideoRendererControl;
class txt_MediaPlayerPlugin;
class QTimerEvent;

namespace core { namespace ubuntu { namespace media {
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
    ~AalMediaPlayerService();

    QMediaControl* requestControl(const char *name);
    void releaseControl(QMediaControl *control);

    AalMediaPlayerControl *mediaPlayerControl() const { return m_mediaPlayerControl; }
    AalVideoRendererControl *videoOutputControl() const { return m_videoOutput; }

    GLConsumerWrapperHybris glConsumer() const;

    bool newMediaPlayer();

    void createVideoSink(uint32_t texture_id);

    QMediaPlayer::AudioRole audioRole() const;
    void setAudioRole(QMediaPlayer::AudioRole audioRole);

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

Q_SIGNALS:
    void serviceReady();

protected:
#ifdef MEASURE_PERFORMANCE
    void measurePerformance();
#endif

private:
    void createMediaPlayerControl();
    void createVideoRendererControl();

    void deleteMediaPlayerControl();
    void deleteVideoRendererControl();

    // Signals the proper QMediaPlayer::Error from a core::ubuntu::media::Error
    void signalQMediaPlayerError(const core::ubuntu::media::Player::Error &error);

    void onPlaybackStatusChanged(const core::ubuntu::media::Player::PlaybackStatus &status);
    void onError(const core::ubuntu::media::Player::Error &error);

    std::shared_ptr<core::ubuntu::media::Service> m_hubService;
    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayerSession;
    core::Connection m_playbackStatusChangedConnection;
    core::Connection m_errorConnection;

    AalMediaPlayerControl *m_mediaPlayerControl;
    AalVideoRendererControl *m_videoOutput;
    bool m_videoOutputReady;

    int64_t m_cachedDuration;

    const QMediaPlaylist* m_mediaPlaylist;

#ifdef MEASURE_PERFORMANCE
    qint64 m_lastFrameDecodeStart;
    qint64 m_currentFrameDecodeStart;
    qint16 m_avgCount;
    qint64 m_frameDecodeAvg;
#endif
};

#endif
