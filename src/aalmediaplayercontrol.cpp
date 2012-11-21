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

#include "media_compatibility_layer.h"

#include <QDebug>

AalMediaPlayerControl::AalMediaPlayerControl(AalMediaPlayerService *service, QObject *parent)
   : QMediaPlayerControl(parent),
    m_service(service),
    m_state(QMediaPlayer::StoppedState),
    m_status(QMediaPlayer::NoMedia)
#if 0
    m_state(QCamera::UnloadedState),
    m_status(QCamera::UnloadedStatus),
    m_captureMode(QCamera::CaptureStillImage)
#endif
{
    bool ok = m_service->newMediaPlayer();
    if (!ok)
    {
        qDebug() << "Failed to create a new hybris media player." << endl;
        return;
    }
}

AalMediaPlayerControl::~AalMediaPlayerControl()
{
}

#if 0
QCamera::State AalMediaPlayerControl::state() const
{
    return m_state;
}

void AalMediaPlayerControl::setState(QCamera::State state)
{
    if (m_state == state)
        return;

    if (m_state == QCamera::UnloadedState) {
        bool ok = m_service->connectCamera();
        if (!ok)
            return;
    }

    qDebug() << m_state << "->" << state;
    m_state = state;
    Q_EMIT stateChanged(m_state);
}

QCamera::Status AalMediaPlayerControl::status() const
{
    return m_status;
}

QCamera::CaptureModes AalMediaPlayerControl::captureMode() const
{
    return m_captureMode;
}

void AalMediaPlayerControl::setCaptureMode(QCamera::CaptureModes mode)
{
    if (m_captureMode == mode)
        return;

    m_captureMode = mode;
    Q_EMIT captureModeChanged(mode);
}

bool AalMediaPlayerControl::isCaptureModeSupported(QCamera::CaptureModes mode) const
{
    return (QCamera::CaptureStillImage==mode) | (QCamera::CaptureVideo==mode);
}

bool AalMediaPlayerControl::canChangeProperty(QCameraControl::PropertyChangeType changeType, QCamera::Status status) const
{
    Q_UNUSED(changeType);
    Q_UNUSED(status);

    return true;
}
#endif

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
}

void AalMediaPlayerControl::setVolume(int)
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

bool AalMediaPlayerControl::isMuted() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
    return false;
}

void AalMediaPlayerControl::setMuted(bool)
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

int AalMediaPlayerControl::bufferStatus() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

bool AalMediaPlayerControl::isAudioAvailable() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

bool AalMediaPlayerControl::isVideoAvailable() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

bool AalMediaPlayerControl::isSeekable() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

QMediaTimeRange AalMediaPlayerControl::availablePlaybackRanges() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

qreal AalMediaPlayerControl::playbackRate() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

void AalMediaPlayerControl::setPlaybackRate(qreal)
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

QMediaContent AalMediaPlayerControl::media() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
}

const QIODevice* AalMediaPlayerControl::mediaStream() const
{
    qDebug() << __PRETTY_FUNCTION__ << endl;
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

