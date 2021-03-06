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

#include "player.h"
#include "service.h"

namespace core {
namespace ubuntu {
namespace media {

const std::shared_ptr<media::Service> TestService::Client::instance()
{
    return std::make_shared<TestService>();
}

std::shared_ptr<Player> TestService::create_session(const Player::Configuration&)
{
    return std::make_shared<TestPlayer>();
}

void TestService::detach_session(const std::string&, const Player::Configuration&)
{
}

std::shared_ptr<Player> TestService::reattach_session(const std::string&, const Player::Configuration&)
{
    return NULL;
}

void TestService::destroy_session(const std::string&, const Player::Configuration&)
{
}

std::shared_ptr<Player> TestService::create_fixed_session(const std::string&, const Player::Configuration&)
{
    return 0;
}

std::shared_ptr<Player> TestService::resume_session(Player::PlayerKey)
{
    return 0;
}

void TestService::pause_other_sessions(Player::PlayerKey)
{
}

const core::Signal<void>& TestService::service_disconnected() const
{
    static const core::Signal<void> s;
    return s;
}

const core::Signal<void>& TestService::service_reconnected() const
{
    static const core::Signal<void> s;
    return s;
}

}
}
}
