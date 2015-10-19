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

#include "service.h"

namespace core {
namespace ubuntu {
namespace media {

const std::shared_ptr<media::Service> TestService::Client::instance()
{
    return NULL;
}

std::shared_ptr<Player> TestService::create_session(const Player::Configuration&)
{
    return NULL;
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

std::shared_ptr<Player> TestService::create_fixed_session(const std::string& name, const Player::Configuration&)
{
    (void) name;
    return 0;
}

std::shared_ptr<Player> TestService::resume_session(Player::PlayerKey)
{
    return 0;
}

void TestService::set_current_player(Player::PlayerKey)
{
}

void TestService::pause_other_sessions(Player::PlayerKey)
{
}

}
}
}
