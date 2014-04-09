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


#include <QtGlobal>
#include <QtTest/QtTest>

using namespace std;

struct core::ubuntu::media::TestPlayer::Configuration
{
};

namespace core {
namespace ubuntu {
namespace media {

typedef void* GLConsumerWrapperHybris;

const TestPlayer::Configuration& TestPlayer::Client::default_configuration()
{
    static const TestPlayer::Configuration config;
    return config;
}

Player::Player()
{
}

Player::~Player()
{
}

TestPlayer::TestPlayer()
{
}

TestPlayer::~TestPlayer()
{
}

shared_ptr<TrackList> TestPlayer::track_list()
{
    static shared_ptr<TrackList> ret(NULL);
    return ret;
}

bool TestPlayer::open_uri(const Track::UriType& uri)
{
    Q_UNUSED(uri);
    return true;
}

void TestPlayer::create_video_sink(uint32_t texture_id)
{
    Q_UNUSED(texture_id);
}

GLConsumerWrapperHybris TestPlayer::gl_consumer() const
{
    return NULL;
}

void TestPlayer::next()
{
}

void TestPlayer::previous()
{
}

void TestPlayer::play()
{
}

void TestPlayer::pause()
{
}

void TestPlayer::stop()
{
}

void TestPlayer::seek_to(const std::chrono::microseconds& offset)
{
    m_position.set(offset.count());
}

void TestPlayer::set_frame_available_callback(FrameAvailableCb cb, void *context)
{
    Q_UNUSED(cb);
    Q_UNUSED(context);
}

const core::Property<bool>& TestPlayer::can_play() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& TestPlayer::can_pause() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& TestPlayer::can_seek() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& TestPlayer::can_go_previous() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& TestPlayer::can_go_next() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& TestPlayer::is_video_source() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<bool>& TestPlayer::is_audio_source() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<TestPlayer::PlaybackStatus>& TestPlayer::playback_status() const
{
    static core::Property<Player::PlaybackStatus> ret(Player::PlaybackStatus::null);
    return ret;
}

const core::Property<Player::LoopStatus>& TestPlayer::loop_status() const
{
    static core::Property<Player::LoopStatus> ret(Player::LoopStatus::none);
    return ret;
}

const core::Property<Player::PlaybackRate>& TestPlayer::playback_rate() const
{
    static core::Property<Player::PlaybackRate> ret(1);
    return ret;
}

const core::Property<bool>& TestPlayer::is_shuffle() const
{
    static core::Property<bool> ret(true);
    return ret;
}

const core::Property<Track::MetaData>& TestPlayer::meta_data_for_current_track() const
{
    static core::Property<Track::MetaData> ret;
    return ret;
}

const core::Property<Player::Volume>& TestPlayer::volume() const
{
    static core::Property<Volume> ret(1);
    return ret;
}

const core::Property<Player::PlaybackRate>& TestPlayer::minimum_playback_rate() const
{
    static core::Property<Volume> ret(1);
    return ret;
}

const core::Property<Player::PlaybackRate>& TestPlayer::maximum_playback_rate() const
{
    static core::Property<Volume> ret(8);
    return ret;
}

const core::Property<uint64_t>& TestPlayer::position() const
{
    return m_position;
}

const core::Property<uint64_t>& TestPlayer::duration() const
{
    static const core::Property<uint64_t> dur(1e6);
    return dur;
}

core::Property<Player::LoopStatus>& TestPlayer::loop_status()
{
    static core::Property<Player::LoopStatus> ret(Player::LoopStatus::none);
    return ret;
}

core::Property<Player::PlaybackRate>& TestPlayer::playback_rate()
{
    static core::Property<Player::PlaybackRate> ret(1);
    return ret;
}

core::Property<bool>& TestPlayer::is_shuffle()
{
    static core::Property<bool> ret(true);
    return ret;
}

core::Property<Player::Volume>& TestPlayer::volume()
{
    static core::Property<Volume> ret(1);
    return ret;
}


const core::Signal<uint64_t>& TestPlayer::seeked_to() const
{
    static core::Signal<uint64_t> ret;
    return ret;
}

const core::Signal<void>& TestPlayer::end_of_stream() const
{
    static core::Signal<void> ret;
    return ret;
}

const std::shared_ptr<Service> Service::Client::instance()
{
    return NULL;
}

const Track::Id& TrackList::after_empty_track()
{
    static const Track::Id track_id;
    return track_id;
}

}
}
}
