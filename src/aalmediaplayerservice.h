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

#include <media/media_compatibility_layer.h>

#include <QMediaService>

class AalMediaPlayerControl;
class QMediaPlayerControl;
class AalVideoRendererControl;

struct MediaPlayerWrapper;

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

    MediaPlayerWrapper *androidControl();

    bool newMediaPlayer();

    void setMedia(const QUrl &url);
    void play();
    void pause();
    void stop();
    int position() const;
    void setPosition(int msec);
    int duration() const;

    int getVolume() const;
    void setVolume(int volume);

    void setVideoTextureNeedsUpdateCb(on_video_texture_needs_update cb, void *context);
    void setVideoSizeCb(on_msg_set_video_size cb, void *context);
    void setPlaybackCompleteCb(on_playback_complete cb, void *context);

    static AalMediaPlayerService *instance() { return m_service; }

private:
    static AalMediaPlayerService *m_service;

    AalMediaPlayerControl *m_mediaPlayerControl;
    AalVideoRendererControl *m_videoOutput;
    MediaPlayerWrapper *m_androidMediaPlayer;

    int m_mediaPlayerControlRef;
    int m_videoOutputRef;
};

#endif
