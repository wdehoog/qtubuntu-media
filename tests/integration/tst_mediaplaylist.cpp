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

#include "tst_mediaplaylist.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QtTest/QtTest>

void tst_MediaPlaylist::initTestCase()
{
}

void tst_MediaPlaylist::cleanupTestCase()
{
}

void tst_MediaPlaylist::addTwoTracksAndVerify()
{
    qDebug() << Q_FUNC_INFO;
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    playlist->addMedia(QUrl(QFINDTESTDATA("testdata/testfile.ogg")));
    playlist->addMedia(QUrl(QFINDTESTDATA("testdata/testfile.mp4")));

    QCOMPARE(playlist->mediaCount(), 2);
}

void tst_MediaPlaylist::addListOfTracksAndVerify()
{
    qDebug() << Q_FUNC_INFO;
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    QList<QMediaContent> content;
    content.push_back(QUrl(QFINDTESTDATA("testdata/testfile.ogg")));
    content.push_back(QUrl(QFINDTESTDATA("testdata/testfile.mp4")));

    playlist->addMedia(content);

    QCOMPARE(playlist->mediaCount(), 2);
}

QTEST_GUILESS_MAIN(tst_MediaPlaylist)
