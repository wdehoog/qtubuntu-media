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

#ifndef TST_MEDIAURIS_H
#define TST_MEDIAURIS_H

#include <core/media/player.h>

#include <deque>
#include <future>
#include <memory>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QObject>

class AalMediaPlaylistControl;
class QMediaPlayer;

class tst_MediaUris : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    // We want the setup to be run prior to every test case to
    // ensure correct test isolation, see http://qt-project.org/doc/qt-5/qtest-overview.html.
    void initTestCase();
    void cleanupTestCase();

    void init();

    void verifySpecialAsciiCharsCanPlaySetMedia();
    void verifySpecialAsciiCharsCanPlayPlaylist();

private:
};

#endif // TST_MEDIAURIS_H
