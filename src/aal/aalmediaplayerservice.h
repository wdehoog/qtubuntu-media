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

#include <core/media/service.h>
#include <core/media/player.h>
#include <core/media/track_list.h>

#include <QMediaPlaylist>
#include <QMediaService>

class AalMediaPlayerControl;
class QMediaPlayerControl;
class AalVideoRendererControl;

class AalMediaPlayerService : public QMediaService
{
    Q_OBJECT
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

    void setMedia(const QUrl &url);
    void setMediaPlaylist(const QMediaPlaylist& playlist);
    void play();
    void pause();
    void stop();
    int position() const;
    void setPosition(int msec);
    int duration() const;
    bool isVideoSource() const;
    bool isAudioSource() const;

    int getVolume() const;
    void setVolume(int volume);

    void pushPlaylist();

    static AalMediaPlayerService *instance() { return m_service; }

Q_SIGNALS:
    void serviceReady();

private:
    static void onFrameAvailableCb(void *context);
    void onFrameAvailable();

    static AalMediaPlayerService *m_service;
    std::shared_ptr<core::ubuntu::media::Service> m_hubService;
    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayerSession;

    AalMediaPlayerControl *m_mediaPlayerControl;
    AalVideoRendererControl *m_videoOutput;
    bool m_videoOutputReady;

    int m_mediaPlayerControlRef;
    int m_videoOutputRef;

    const QMediaPlaylist* m_mediaPlaylist;
};

#endif
