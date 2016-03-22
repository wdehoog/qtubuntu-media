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

#include "tst_audiorole.h"

#include <unistd.h>

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QtTest/QtTest>

void tst_AudioRole::initTestCase()
{
}

void tst_AudioRole::cleanupTestCase()
{
}

void tst_AudioRole::init()
{
}

void tst_AudioRole::verifyAudioRolePlays()
{
    QMediaPlayer *player = new QMediaPlayer;
    const QString fullFileUri {"file://" + QFINDTESTDATA("testdata/Ubuntu.ogg")};
    const QMediaContent content {QUrl{fullFileUri}};
    qDebug() << "Trying to play file:" << fullFileUri;
    player->setMedia(content);
    QSignalSpy spy(player, SIGNAL(audioRoleChanged(QAudio::Role)));
    player->setAudioRole(QAudio::MusicRole);
    QCOMPARE(spy.count(), 1);

    player->play();
    sleep(1);
    qDebug() << "player->error():" << player->error();
    qDebug() << "player->mediaStatus():" << player->mediaStatus();
    QCOMPARE(player->audioRole(), QAudio::MusicRole);
    QVERIFY(player->mediaStatus() != QMediaPlayer::InvalidMedia);
    QCOMPARE(player->state(), QMediaPlayer::State::PlayingState);
}

void tst_AudioRole::verifyVideoRolePlays()
{
    QMediaPlayer *player = new QMediaPlayer;
    const QString fullFileUri {"file://" + QFINDTESTDATA("testdata/Ubuntu.ogg")};
    const QMediaContent content {QUrl{fullFileUri}};
    qDebug() << "Trying to play file:" << fullFileUri;
    player->setMedia(content);
    QSignalSpy spy(player, SIGNAL(audioRoleChanged(QAudio::Role)));
    player->setAudioRole(QAudio::VideoRole);
    QCOMPARE(spy.count(), 1);

    player->play();
    sleep(1);
    qDebug() << "player->error():" << player->error();
    qDebug() << "player->mediaStatus():" << player->mediaStatus();
    QCOMPARE(player->audioRole(), QAudio::VideoRole);
    QVERIFY(player->mediaStatus() != QMediaPlayer::InvalidMedia);
    QCOMPARE(player->state(), QMediaPlayer::State::PlayingState);
}

void tst_AudioRole::verifyAlarmRolePlays()
{
    QMediaPlayer *player = new QMediaPlayer;
    const QString fullFileUri {"file://" + QFINDTESTDATA("testdata/Ubuntu.ogg")};
    const QMediaContent content {QUrl{fullFileUri}};
    qDebug() << "Trying to play file:" << fullFileUri;
    player->setMedia(content);
    QSignalSpy spy(player, SIGNAL(audioRoleChanged(QAudio::Role)));
    player->setAudioRole(QAudio::AlarmRole);
    QCOMPARE(spy.count(), 1);

    player->play();
    sleep(1);
    qDebug() << "player->error():" << player->error();
    qDebug() << "player->mediaStatus():" << player->mediaStatus();
    QCOMPARE(player->audioRole(), QAudio::AlarmRole);
    QVERIFY(player->mediaStatus() != QMediaPlayer::InvalidMedia);
    QCOMPARE(player->state(), QMediaPlayer::State::PlayingState);
}

void tst_AudioRole::verifyNotificationRolePlays()
{
    QMediaPlayer *player = new QMediaPlayer;
    const QString fullFileUri {"file://" + QFINDTESTDATA("testdata/Ubuntu.ogg")};
    const QMediaContent content {QUrl{fullFileUri}};
    qDebug() << "Trying to play file:" << fullFileUri;
    player->setMedia(content);
    QSignalSpy spy(player, SIGNAL(audioRoleChanged(QAudio::Role)));
    player->setAudioRole(QAudio::NotificationRole);
    QCOMPARE(spy.count(), 1);

    player->play();
    sleep(1);
    qDebug() << "player->error():" << player->error();
    qDebug() << "player->mediaStatus():" << player->mediaStatus();
    QCOMPARE(player->audioRole(), QAudio::NotificationRole);
    QVERIFY(player->mediaStatus() != QMediaPlayer::InvalidMedia);
    QCOMPARE(player->state(), QMediaPlayer::State::PlayingState);
}

void tst_AudioRole::verifyVoiceCommunicationRolePlays()
{
    QMediaPlayer *player = new QMediaPlayer;
    const QString fullFileUri {"file://" + QFINDTESTDATA("testdata/Ubuntu.ogg")};
    const QMediaContent content {QUrl{fullFileUri}};
    qDebug() << "Trying to play file:" << fullFileUri;
    player->setMedia(content);
    QSignalSpy spy(player, SIGNAL(audioRoleChanged(QAudio::Role)));
    player->setAudioRole(QAudio::VoiceCommunicationRole);
    QCOMPARE(spy.count(), 1);

    player->play();
    sleep(1);
    qDebug() << "player->error():" << player->error();
    qDebug() << "player->mediaStatus():" << player->mediaStatus();
    QCOMPARE(player->audioRole(), QAudio::VoiceCommunicationRole);
    QVERIFY(player->mediaStatus() != QMediaPlayer::InvalidMedia);
    QCOMPARE(player->state(), QMediaPlayer::State::PlayingState);
}

QTEST_GUILESS_MAIN(tst_AudioRole)
