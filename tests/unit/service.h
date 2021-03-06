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

#ifndef SERVICE_H_
#define SERVICE_H_

#include <core/media/player.h>
#include <core/media/service.h>

#include <memory>

namespace core
{
namespace ubuntu
{
namespace media
{
class TestService : public Service
{
  public:
    struct Client
    {
        static const std::shared_ptr<Service> instance();
    };

    TestService() = default;

    TestService(const TestService&) = delete;
    virtual ~TestService() = default;

    TestService& operator=(const TestService&) = delete;
    bool operator==(const TestService&) const = delete;

    virtual std::shared_ptr<Player> create_session(const Player::Configuration&);
    virtual void detach_session(const std::string& uuid, const Player::Configuration&);
    virtual std::shared_ptr<Player> reattach_session(const std::string& uuid, const Player::Configuration&);
    virtual void destroy_session(const std::string& uuid, const Player::Configuration&);
    virtual std::shared_ptr<Player> create_fixed_session(const std::string& name, const Player::Configuration&);
    virtual std::shared_ptr<Player> resume_session(Player::PlayerKey);
    virtual void pause_other_sessions(Player::PlayerKey);

    virtual const core::Signal<void>& service_disconnected() const;
    virtual const core::Signal<void>& service_reconnected() const;
};
}
}
}

#endif
