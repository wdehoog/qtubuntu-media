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
#include "aalutility.h"

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

    delete playlist;
    delete player;
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

    delete playlist;
    delete player;
}

void tst_MediaPlaylist::goToNextTrack()
{
    qDebug() << Q_FUNC_INFO;
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    const QUrl audio(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    const QUrl video(QUrl("file://" + QFINDTESTDATA("testdata/testfile.mp4")));
    qDebug() << "audio URL: " << audio.toString();
    qDebug() << "video URL: " << video.toString();
    playlist->addMedia(audio);
    playlist->addMedia(video);

    QCOMPARE(playlist->mediaCount(), 2);

    player->play();

    const QUrl audioToVerify(playlist->currentMedia().canonicalUrl());
    QCOMPARE(audioToVerify, audio);

    playlist->next();

    const QUrl videoToVerify(playlist->currentMedia().canonicalUrl());
    QCOMPARE(videoToVerify, video);

    delete playlist;
    delete player;
}

void tst_MediaPlaylist::goToPreviousTrack()
{
#if 0
    qDebug() << Q_FUNC_INFO;
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    playlist->addMedia(QUrl(QFINDTESTDATA("testdata/testfile.ogg")));
    playlist->addMedia(QUrl(QFINDTESTDATA("testdata/testfile.mp4")));

    QCOMPARE(playlist->mediaCount(), 2);

    player->play();
    playlist->previous();

    delete playlist;
    delete player;
#endif
}

void tst_MediaPlaylist::verifyMedia()
{
    qDebug() << Q_FUNC_INFO;
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    const QUrl audio(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    const QUrl video(QUrl("file://" + QFINDTESTDATA("testdata/testfile.mp4")));
    qDebug() << "audio URL: " << audio.toString();
    qDebug() << "video URL: " << video.toString();
    playlist->addMedia(audio);
    playlist->addMedia(video);

    QCOMPARE(playlist->mediaCount(), 2);

    const QUrl audioToVerify(playlist->media(0).canonicalUrl());
    QCOMPARE(audioToVerify, audio);

    const QUrl videoToVerify(playlist->media(1).canonicalUrl());
    QCOMPARE(videoToVerify, video);

    delete playlist;
    delete player;
}

void tst_MediaPlaylist::removeTrackAndVerify()
{
    qDebug() << Q_FUNC_INFO;
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    playlist->addMedia(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    const QUrl video(QUrl("file://" + QFINDTESTDATA("testdata/testfile.mp4")));
    playlist->addMedia(video);

    QCOMPARE(playlist->mediaCount(), 2);

    playlist->removeMedia(0);

    QCOMPARE(playlist->mediaCount(), 1);

    const QUrl videoToVerify(playlist->media(0).canonicalUrl());
    QCOMPARE(videoToVerify, video);

    delete playlist;
    delete player;
}

QTEST_GUILESS_MAIN(tst_MediaPlaylist)
