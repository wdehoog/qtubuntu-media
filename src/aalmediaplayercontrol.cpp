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

    m_cachedVolume = volume();
}

AalMediaPlayerControl::~AalMediaPlayerControl()
{
    stop();
    m_state = QMediaPlayer::StoppedState;
    m_status = QMediaPlayer::NoMedia;
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

    m_status = QMediaPlayer::LoadingMedia;

    m_service->setMedia(media.canonicalUrl());

    m_status = QMediaPlayer::LoadedMedia;
}

void AalMediaPlayerControl::play()
{
    qDebug() << __PRETTY_FUNCTION__ << endl;

    m_service->play();

    m_state = QMediaPlayer::PlayingState;
    Q_EMIT stateChanged(m_state);
}

void AalMediaPlayerControl::pause()
{
    qDebug() << __PRETTY_FUNCTION__ << endl;

    m_service->pause();

    m_state = QMediaPlayer::PausedState;
    Q_EMIT stateChanged(m_state);
}

void AalMediaPlayerControl::stop()
{
    qDebug() << __PRETTY_FUNCTION__ << endl;

    m_service->stop();

    m_state = QMediaPlayer::StoppedState;
    Q_EMIT stateChanged(m_state);
}

