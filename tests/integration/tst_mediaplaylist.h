/*
 * Copyright (C) 2015 Canonical, Ltd.
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

#ifndef TST_MEDIAPLAYLIST_H
#define TST_MEDIAPLAYLIST_H

#include <core/media/player.h>

#include <deque>
#include <future>
#include <memory>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QObject>

class AalMediaPlaylistControl;
class QMediaPlayer;

class tst_MediaPlaylist : public QObject
{
    Q_OBJECT

    AalMediaPlaylistControl *m_mediaPlaylistControl;

public:
    enum Signals {
        Unknown,
        CurrentMediaChanged,
        MediaInserted,
        MediaRemoved
    };

Q_SIGNALS:

private Q_SLOTS:
    // We want the setup to be run prior to every test case to
    // ensure correct test isolation, see http://qt-project.org/doc/qt-5/qtest-overview.html.
    void initTestCase();
    void cleanupTestCase();

    void init();

    void constructDestroyRepeat();

    void addTwoTracksAndVerify();
    void insertTracksAtPositionAndVerify();
    void moveTrackAndVerify();
    void addListOfTracksAndVerify();
    void addLargeListOfTracksAndVerify();
    void addLargeListOfTracksAtOnceAndVerify();
    void addTwoListsOfTracksAtOnceAndVerify();

    void goToNextTrack();
    void goToPreviousTrack();
    void verifyMedia();

    void removeTrackAndVerify();

    void verifyCurrentIndex();
    void verifyNextIndex();
    void verifyPreviousIndex();

    void verifyPlaybackModeCurrentItemInLoop();
    void verifyPlaybackModeSequential();

    void playReusePlayTrackList();

private:
    std::deque<Signals> m_signalsDeque;

    template<typename R>
        bool is_ready(std::future<R> const& f)
        { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

    template<typename R>
        void wait_for_signal(std::future<R> const& f);

    void waitTrackChange(QMediaPlaylist *playlist);
    void waitTrackInserted(QMediaPlaylist *playlist);
    void waitTrackRemoved(QMediaPlaylist *playlist);
    void waitPlaybackModeChange(QMediaPlaylist *playlist,
                                const std::function<void()>& action);

    // A generic way of getting a signal registered into m_signalsDeque without blocking
    // which can be used to later check the order of signals that were emitted. Simply call
    // this method for each signal that you'd like to check and it'll be pushed onto the deque
    // when it's fired.
    void connectSignal(QMediaPlaylist *playlist, Signals signal);
};

#endif // TST_MEDIAPLAYLIST_H
