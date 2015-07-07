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

#include <thread>
#include <unistd.h>

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QtTest/QtTest>

//#define DISABLE_TEST

void tst_MediaPlaylist::initTestCase()
{
}

void tst_MediaPlaylist::cleanupTestCase()
{
}

void tst_MediaPlaylist::init()
{
    // NOTE: This sleep is currently needed in order to give media-hub a bit of time
    // between our different tests to cleanup and come back in a state where it can
    // respond to our requests.
    sleep(1);
}

void tst_MediaPlaylist::addTwoTracksAndVerify()
{
#ifndef DISABLE_TEST
    qDebug() << Q_FUNC_INFO;
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    playlist->addMedia(QUrl(QFINDTESTDATA("testdata/testfile.ogg")));
    playlist->addMedia(QUrl(QFINDTESTDATA("testdata/testfile.mp4")));

    QCOMPARE(playlist->mediaCount(), 2);

    delete playlist;
    delete player;
#endif
}

void tst_MediaPlaylist::addListOfTracksAndVerify()
{
#ifndef DISABLE_TEST
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
#endif
}

void tst_MediaPlaylist::goToNextTrack()
{
#ifndef DISABLE_TEST
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

    QCoreApplication::processEvents();

    const QUrl audioToVerify(playlist->currentMedia().canonicalUrl());
    QCOMPARE(audioToVerify, audio);

    playlist->next();

    QCoreApplication::processEvents();

    const QUrl videoToVerify(playlist->currentMedia().canonicalUrl());
    QCOMPARE(videoToVerify, video);

    delete playlist;
    delete player;
#endif
}

void tst_MediaPlaylist::goToPreviousTrack()
{
#ifndef DISABLE_TEST
    qDebug() << Q_FUNC_INFO;
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    const QUrl audio1(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    const QUrl audio2(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    playlist->addMedia(audio1);
    playlist->addMedia(audio2);

    QCOMPARE(playlist->mediaCount(), 2);
    playlist->setCurrentIndex(1);

    player->play();

    QCoreApplication::processEvents();

    const QUrl audio2ToVerify(playlist->currentMedia().canonicalUrl());
    QCOMPARE(audio2ToVerify, audio2);

    playlist->previous();

    QCoreApplication::processEvents();

    const QUrl audio1ToVerify(playlist->currentMedia().canonicalUrl());
    QCOMPARE(audio2ToVerify, audio1);
    QCOMPARE(playlist->currentIndex(), 0);

    delete playlist;
    delete player;
#endif
}

void tst_MediaPlaylist::verifyMedia()
{
#ifndef DISABLE_TEST
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
#endif
}

void tst_MediaPlaylist::removeTrackAndVerify()
{
#ifndef DISABLE_TEST
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
#endif
}

void tst_MediaPlaylist::verifyCurrentIndex()
{
#ifndef DISABLE_TEST
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    QList<QMediaContent> content;
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.mp4")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    playlist->addMedia(content);

    QCOMPARE(playlist->mediaCount(), 3);

    playlist->setCurrentIndex(1);

    QCoreApplication::processEvents();

    QCOMPARE(playlist->currentIndex(), 1);

    delete playlist;
    delete player;
#endif
}

void tst_MediaPlaylist::verifyNextIndex()
{
#ifndef DISABLE_TEST
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    QList<QMediaContent> content;
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.mp4")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.mp4")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.mp4")));
    playlist->addMedia(content);

    QCoreApplication::processEvents();

    QCOMPARE(playlist->mediaCount(), 6);

    QCOMPARE(playlist->nextIndex(1), 1);
    QCOMPARE(playlist->nextIndex(4), 4);
    QCOMPARE(playlist->nextIndex(6), 0);
    QCOMPARE(playlist->nextIndex(7), 1);
    QCOMPARE(playlist->nextIndex(11), 5);

    delete playlist;
    delete player;
#endif
}

void tst_MediaPlaylist::verifyPlaybackModeCurrentItemInLoop()
{
#ifndef DISABLE_TEST
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    QList<QMediaContent> content;
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    playlist->addMedia(content);

    QCOMPARE(playlist->mediaCount(), 2);

    QMediaContent current_media;
    std::promise<QMediaContent> promise;
    std::future<QMediaContent> future{promise.get_future()};
    QMetaObject::Connection c = connect(playlist, &QMediaPlaylist::currentMediaChanged, [&](const QMediaContent& content)
    {
            qDebug() << "currentMediaChanged to: " << content.canonicalUrl().toString();
            current_media = content;
            promise.set_value(current_media);
    });

    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    player->play();

    while (!is_ready<QMediaContent>(future))
    {
        // Make sure we don't block the main QEventLoop, which
        // would hinder receiving the currentMediaChanged event above
        QCoreApplication::processEvents();
        std::this_thread::yield();
    }

    QCOMPARE(playlist->currentIndex(), 0);

    QObject::disconnect(c);

    delete playlist;
    delete player;
#endif
}

void tst_MediaPlaylist::verifyPlaybackModeSequential()
{
#ifndef DISABLE_TEST
    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    QMediaContent current_media;
    std::promise<QMediaContent> promise;
    std::future<QMediaContent> future{promise.get_future()};

    QMetaObject::Connection c = connect(playlist, &QMediaPlaylist::currentMediaChanged, [&](const QMediaContent& content)
    {
            qDebug() << "currentMediaChanged to: " << content.canonicalUrl().toString();
            current_media = content;
            promise.set_value(current_media);
    });

    QList<QMediaContent> content;
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    content.push_back(QUrl("file://" + QFINDTESTDATA("testdata/testfile.ogg")));
    playlist->addMedia(content);

    QCOMPARE(playlist->mediaCount(), 2);

    playlist->setPlaybackMode(QMediaPlaylist::Sequential);

    player->play();

    while (!is_ready<QMediaContent>(future))
    {
        // Make sure we don't block the main QEventLoop, which
        // would hinder receiving the currentMediaChanged event above
        QCoreApplication::processEvents();
        std::this_thread::yield();
    }

    QCOMPARE(playlist->currentIndex(), 1);

    QObject::disconnect(c);

    delete playlist;
    delete player;
#endif
}

QTEST_GUILESS_MAIN(tst_MediaPlaylist)
