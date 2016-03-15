/*
 * Copyright (C) 2016 Canonical, Ltd.
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

#include "aalaudiorolecontrol.h"

#include <QDebug>

namespace media = core::ubuntu::media;

AalAudioRoleControl::AalAudioRoleControl()
    : QAudioRoleControl()
    , m_audioRole(QAudio::UnknownRole)
{
}

QAudio::Role AalAudioRoleControl::audioRole() const
{
    return m_audioRole;
}

void AalAudioRoleControl::setAudioRole(QAudio::Role role)
{
    if (m_hubPlayerSession == nullptr)
    {
        qWarning() << "Failed to setAudioRole since m_hubPlayerSession is NULL";
        return;
    }

    try {
        m_hubPlayerSession->audio_stream_role().set(fromQAudioRole(role));
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to set audio stream role: " << e.what();
        return;
    }

    if (role != m_audioRole)
        Q_EMIT audioRoleChanged(m_audioRole = role);
}

QList<QAudio::Role> AalAudioRoleControl::supportedAudioRoles() const
{
    return QList<QAudio::Role>() << QAudio::MusicRole
                                 << QAudio::VideoRole
                                 << QAudio::AlarmRole
                                 << QAudio::NotificationRole
                                 << QAudio::VoiceCommunicationRole;
}

void AalAudioRoleControl::setPlayerSession
        (const std::shared_ptr<core::ubuntu::media::Player>& playerSession)
{
    m_hubPlayerSession = playerSession;
}

QAudio::Role AalAudioRoleControl::toQAudioRole(const media::Player::AudioStreamRole &role)
{
    switch (role)
    {
        case media::Player::AudioStreamRole::multimedia:
            return QAudio::MusicRole;
        case media::Player::AudioStreamRole::alarm:
            return QAudio::AlarmRole;
        case media::Player::AudioStreamRole::alert:
            return QAudio::NotificationRole;
        case media::Player::AudioStreamRole::phone:
            return QAudio::VoiceCommunicationRole;
        default:
            qWarning() << "Unhandled or invalid core::ubuntu::media::AudioStreamRole: " << role;
            return QAudio::MusicRole;
    }
}

media::Player::AudioStreamRole AalAudioRoleControl::fromQAudioRole(const QAudio::Role &role)
{
    // If we don't have a valid role, this should be the default translation
    if (role == QAudio::Role::UnknownRole)
        return media::Player::AudioStreamRole::multimedia;

    switch (role)
    {
        case QAudio::MusicRole:
        case QAudio::VideoRole:
            return media::Player::AudioStreamRole::multimedia;
        case QAudio::AlarmRole:
            return media::Player::AudioStreamRole::alarm;
        case QAudio::NotificationRole:
            return media::Player::AudioStreamRole::alert;
        case QAudio::VoiceCommunicationRole:
            return media::Player::AudioStreamRole::phone;
        default:
            qWarning() << "Unhandled or invalid QAudio::Role:" << role;
            return media::Player::AudioStreamRole::multimedia;
    }
}
