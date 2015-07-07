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

Q_SIGNALS:

private Q_SLOTS:
    // We want the setup to be run prior to every test case to
    // ensure correct test isolation, see http://qt-project.org/doc/qt-5/qtest-overview.html.
    void initTestCase();
    void cleanupTestCase();

    void init();

    void addTwoTracksAndVerify();
    void addListOfTracksAndVerify();

    void goToNextTrack();
    void goToPreviousTrack();
    void verifyMedia();

    void removeTrackAndVerify();

    void verifyCurrentIndex();
    void verifyNextIndex();

    void verifyPlaybackModeCurrentItemInLoop();
    void verifyPlaybackModeSequential();

private:
    template<typename R>
        bool is_ready(std::future<R> const& f)
        { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }
};

#endif // TST_MEDIAPLAYLIST_H
