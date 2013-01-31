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

#include <media/media_compatibility_layer.h>

#include <QDebug>

AalMediaPlayerControl::AalMediaPlayerControl(AalMediaPlayerService *service, QObject *parent)
   : QMediaPlayerControl(parent),
    m_service(service),
    m_state(QMediaPlayer::StoppedState),
    m_status(QMediaPlayer::NoMedia)
{
    bool ok = m_service->newMediaPlayer();
    if (!ok)
    {
        qDebug() << "Failed to create a new hybris media player." << endl;
        return;
    }

    m_service->setPlaybackCompleteCb(AalMediaPlayerControl::playbackCompleteCb, static_cast<void *>(this));
    m_service->setMediaPreparedCb(AalMediaPlayerControl::mediaPreparedCb, static_cast<void *>(this));

    m_cachedVolume = volume();
}

AalMediaPlayerControl::~AalMediaPlayerControl()
{
    stop();
    m_state = QMediaPlayer::StoppedState;
    m_status = QMediaPlayer::NoMedia;
    m_cachedVolume = 0;
}

QMediaPlayer::State AalMediaPlayerControl::state() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return m_state;
}

QMediaPlayer::MediaStatus AalMediaPlayerControl::mediaStatus() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return m_status;
}

qint64 AalMediaPlayerControl::duration() const
{
    return static_cast<qint64>(m_service->duration());
}

qint64 AalMediaPlayerControl::position() const
{
    return static_cast<qint64>(m_service->position());
}

void AalMediaPlayerControl::setPosition(qint64 msec)
{
    m_service->setPosition(static_cast<int>(msec));
    Q_EMIT positionChanged(msec);
}

int AalMediaPlayerControl::volume() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return m_service->getVolume();
}

void AalMediaPlayerControl::setVolume(int volume)
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    m_cachedVolume = volume;
    m_service->setVolume(volume);
    Q_EMIT volumeChanged(m_cachedVolume);
}

bool AalMediaPlayerControl::isMuted() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return (volume() == 0);
}

void AalMediaPlayerControl::setMuted(bool muted)
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    if (muted)
    {
        m_cachedVolume = volume();
        setVolume(0);
    }
    else
    {
        setVolume(m_cachedVolume);
    }

    Q_EMIT mutedChanged(muted);
}

int AalMediaPlayerControl::bufferStatus() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    // Until we are playing network streams, there is no buffering necessary
    return 100;
}

bool AalMediaPlayerControl::isAudioAvailable() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return true;
}

bool AalMediaPlayerControl::isVideoAvailable() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return true;
}

bool AalMediaPlayerControl::isSeekable() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return true;
}

QMediaTimeRange AalMediaPlayerControl::availablePlaybackRanges() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    // TODO: this will need to change once we can play networked sources
    return QMediaTimeRange(0, duration());
}

qreal AalMediaPlayerControl::playbackRate() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return 1.0;
}

void AalMediaPlayerControl::setPlaybackRate(qreal rate)
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    Q_UNUSED(rate);
}

QMediaContent AalMediaPlayerControl::media() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return m_mediaContent;
}

const QIODevice* AalMediaPlayerControl::mediaStream() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    // This is only valid if a stream was passed into setMedia()
    return NULL;
}

void AalMediaPlayerControl::setMedia(const QMediaContent& media, QIODevice* stream)
{
    Q_UNUSED(stream);
    qDebug() << __PRETTY_FUNCTION__ << endl;

    stop();

    m_mediaContent = media;
    Q_EMIT mediaChanged(m_mediaContent);

    // Make sure we can actually load something valid
    if (!media.isNull())
    {
        setMediaStatus(QMediaPlayer::LoadingMedia);

        m_service->setMedia(media.canonicalUrl());
    }
}

void AalMediaPlayerControl::play()
{
    m_service->play();

    setState(QMediaPlayer::PlayingState);
}

void AalMediaPlayerControl::pause()
{
    m_service->pause();

    setState(QMediaPlayer::PausedState);
}

void AalMediaPlayerControl::stop()
{
    m_service->stop();

    setState(QMediaPlayer::StoppedState);
}

void AalMediaPlayerControl::playbackCompleteCb(void *context)
{
    if (context != NULL)
        static_cast<AalMediaPlayerControl *>(context)->playbackComplete();
    else
        qWarning() << "Failed to call playbackComplete() since context is NULL." << endl;
}

void AalMediaPlayerControl::playbackComplete()
{
    setMediaStatus(QMediaPlayer::EndOfMedia);
    setState(QMediaPlayer::StoppedState);
}

void AalMediaPlayerControl::mediaPreparedCb(void *context)
{
    if (context != NULL)
        static_cast<AalMediaPlayerControl *>(context)->mediaPrepared();
    else
        qWarning() << "Failed to call mediaPrepared() since context is NULL." << endl;
}

void AalMediaPlayerControl::mediaPrepared()
{
    setMediaStatus(QMediaPlayer::LoadedMedia);
    Q_EMIT durationChanged(duration());
    Q_EMIT positionChanged(position());
}

void AalMediaPlayerControl::setMediaStatus(QMediaPlayer::MediaStatus status)
{
    if (status != m_status)
    {
        m_status = status;
        Q_EMIT mediaStatusChanged(m_status);
    }
}

void AalMediaPlayerControl::setState(QMediaPlayer::State state)
{
    if (state != m_state)
    {
        m_state = state;
        Q_EMIT stateChanged(m_state);
    }
}
