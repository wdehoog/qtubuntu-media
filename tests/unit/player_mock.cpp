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

#include "player_mock.h"

#include <QtGlobal>
#include <QtTest/QtTest>

using namespace std;

struct core::ubuntu::media::PlayerMock::Configuration
{
};

namespace core {
namespace ubuntu {
namespace media {

typedef void* GLConsumerWrapperHybris;

PlayerMock::PlayerMock()
{
}

PlayerMock::~PlayerMock()
{
}

std::string PlayerMock::uuid() const
{
    return std::string{};
}

void PlayerMock::reconnect()
{
}

void PlayerMock::abandon()
{
}

shared_ptr<TrackList> PlayerMock::track_list()
{
    static shared_ptr<TrackList> ret(NULL);
    return ret;
}

Player::PlayerKey PlayerMock::key() const
{
    return 0;
}

bool PlayerMock::open_uri(const Track::UriType& uri)
{
    Q_UNUSED(uri);
    return true;
}

bool PlayerMock::open_uri(const Track::UriType& uri, const HeadersType&)
{
    Q_UNUSED(uri);
    return true;
}

std::shared_ptr<core::ubuntu::media::video::Sink> PlayerMock::create_gl_texture_video_sink(uint32_t texture_id)
{
    Q_UNUSED(texture_id); throw std::runtime_error{"Not implemented"};
}

void PlayerMock::next()
{
}

void PlayerMock::previous()
{
}

void PlayerMock::play()
{
}

void PlayerMock::pause()
{
}

void PlayerMock::stop()
{
}

void PlayerMock::seek_to(const std::chrono::microseconds& offset)
{
    m_position.set(offset.count());
}

const core::Property<bool>& PlayerMock::can_play() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& PlayerMock::can_pause() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& PlayerMock::can_seek() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& PlayerMock::can_go_previous() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& PlayerMock::can_go_next() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& PlayerMock::is_video_source() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& PlayerMock::is_audio_source() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<PlayerMock::PlaybackStatus>& PlayerMock::playback_status() const
{
    static core::Property<Player::PlaybackStatus> ret(Player::PlaybackStatus::null);
    return ret;
}

const core::Property<PlayerMock::Orientation>& PlayerMock::orientation() const
{
    static core::Property<Player::Orientation> ret(Player::Orientation::rotate0);
    return ret;
}

const core::Property<Player::LoopStatus>& PlayerMock::loop_status() const
{
    static core::Property<Player::LoopStatus> ret(Player::LoopStatus::none);
    return ret;
}

const core::Property<Player::PlaybackRate>& PlayerMock::playback_rate() const
{
    static core::Property<Player::PlaybackRate> ret(1);
    return ret;
}

const core::Property<bool>& PlayerMock::shuffle() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<Track::MetaData>& PlayerMock::meta_data_for_current_track() const
{
    static core::Property<Track::MetaData> ret;
    return ret;
}

const core::Property<Player::Volume>& PlayerMock::volume() const
{
    static core::Property<Volume> ret(1);
    return ret;
}

const core::Property<Player::PlaybackRate>& PlayerMock::minimum_playback_rate() const
{
    static core::Property<Volume> ret(1);
    return ret;
}

const core::Property<Player::PlaybackRate>& PlayerMock::maximum_playback_rate() const
{
    static core::Property<Volume> ret(8);
    return ret;
}

const core::Property<int64_t>& PlayerMock::position() const
{
    return m_position;
}

const core::Property<int64_t>& PlayerMock::duration() const
{
    static const core::Property<int64_t> dur(1e6);
    return dur;
}

const core::Property<Player::AudioStreamRole>& PlayerMock::audio_stream_role() const
{
    static const core::Property<Player::AudioStreamRole> role(Player::AudioStreamRole::multimedia);
    return role;
}

const core::Property<Player::Lifetime>& PlayerMock::lifetime() const
{
    static const core::Property<Player::Lifetime> lifetime(Player::Lifetime::normal);
    return lifetime;
}

core::Property<Player::LoopStatus>& PlayerMock::loop_status()
{
    static core::Property<Player::LoopStatus> ret(Player::LoopStatus::none);
    return ret;
}

core::Property<Player::PlaybackRate>& PlayerMock::playback_rate()
{
    static core::Property<Player::PlaybackRate> ret(1);
    return ret;
}

core::Property<bool>& PlayerMock::shuffle()
{
    static core::Property<bool> ret(true);
    return ret;
}

core::Property<Player::Volume>& PlayerMock::volume()
{
    static core::Property<Volume> ret(1);
    return ret;
}

core::Property<Player::AudioStreamRole>& PlayerMock::audio_stream_role()
{
    static core::Property<Player::AudioStreamRole> role(Player::AudioStreamRole::multimedia);
    return role;
}

core::Property<Player::Lifetime>& PlayerMock::lifetime()
{
    static core::Property<Player::Lifetime> lifetime(Player::Lifetime::normal);
    return lifetime;
}

const core::Signal<int64_t>& PlayerMock::seeked_to() const
{
    static core::Signal<int64_t> ret;
    return ret;
}

const core::Signal<void>& PlayerMock::about_to_finish() const
{
    static core::Signal<void> ret;
    return ret;
}

const core::Signal<void>& PlayerMock::end_of_stream() const
{
    static core::Signal<void> ret;
    return ret;
}

const core::Signal<Player::PlaybackStatus>& PlayerMock::playback_status_changed() const
{
    static core::Signal<Player::PlaybackStatus> ret;
    return ret;
}

core::Signal<Player::PlaybackStatus>& PlayerMock::playback_status_changed()
{
    static core::Signal<Player::PlaybackStatus> ret;
    return ret;
}

const core::Signal<core::ubuntu::media::video::Dimensions>& PlayerMock::video_dimension_changed() const
{
    static core::Signal<core::ubuntu::media::video::Dimensions> ret;
    return ret;
}

const core::Signal<uint64_t>& PlayerMock::duration_changed() const
{
    static core::Signal<uint64_t> ret;
    return ret;
}

const core::Signal<Player::Error>& PlayerMock::error() const
{
    static core::Signal<Player::Error> ret;
    return ret;
}

}
}
}
