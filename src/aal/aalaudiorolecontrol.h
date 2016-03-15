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

#ifndef AALAUDIOROLECONTROL_H
#define AALAUDIOROLECONTROL_H

#include <core/media/player.h>

#include <qaudiorolecontrol.h>

class AalAudioRoleControl : public QAudioRoleControl
{
public:
    AalAudioRoleControl();

    QAudio::Role audioRole() const;
    void setAudioRole(QAudio::Role role);
    QList<QAudio::Role> supportedAudioRoles() const;

    static QAudio::Role toQAudioRole
        (const core::ubuntu::media::Player::AudioStreamRole &role);
    static core::ubuntu::media::Player::AudioStreamRole fromQAudioRole
        (const QAudio::Role &role);

    void setPlayerSession(const std::shared_ptr<core::ubuntu::media::Player>& playerSession);

private:
    QAudio::Role m_audioRole;
    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayerSession;
};

#endif // AALAUDIOROLECONTROL_H
