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

#ifndef AALMEDIAPLAYER_H
#define AALMEDIAPLAYER_H

#include <QMediaPlayerControl>

class AalMediaPlayerService;

class AalMediaPlayerControl : public QMediaPlayerControl
{
Q_OBJECT
public:
    AalMediaPlayerControl(AalMediaPlayerService *service, QObject *parent = 0);
    ~AalMediaPlayerControl();

    bool eventFilter(QObject *obj, QEvent *event);
    
    virtual QMediaPlayer::State state() const;
    virtual QMediaPlayer::MediaStatus mediaStatus() const;
    virtual qint64 duration() const;
    virtual qint64 position() const;
    virtual void setPosition(qint64);
    virtual int volume() const;
    virtual void setVolume(int);
    virtual bool isMuted() const;
    virtual void setMuted(bool);
    virtual int bufferStatus() const;
    virtual bool isAudioAvailable() const;
    virtual bool isVideoAvailable() const;
    virtual bool isSeekable() const;
    virtual QMediaTimeRange availablePlaybackRanges() const;
    virtual qreal playbackRate() const;
    virtual void setPlaybackRate(qreal);
    virtual QMediaContent media() const;
    virtual const QIODevice* mediaStream() const;
    virtual void setMedia(const QMediaContent&, QIODevice*);
    virtual void play();
    virtual void pause();
    virtual void stop();

    static void playbackCompleteCb(void *context);
    void mediaPrepared();

private:
    AalMediaPlayerService *m_service;
    QMediaPlayer::State m_state;
    QMediaPlayer::MediaStatus m_status;
    QMediaContent m_mediaContent;
    int m_cachedVolume;
    bool m_applicationActive;

    void playbackComplete();

    void setMediaStatus(QMediaPlayer::MediaStatus status);
    void setState(QMediaPlayer::State state);
};

#endif
