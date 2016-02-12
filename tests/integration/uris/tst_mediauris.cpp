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

#include "tst_mediauris.h"

#include <unistd.h>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QProcess>

#include <QtTest/QtTest>

void tst_MediaUris::initTestCase()
{
    QProcess p;
    qDebug() << "Current working dir:" << QDir::currentPath();
    p.start("../../../../tests/integration/uris/generate_test_files.sh",
            QStringList() << QString{QDir::currentPath() + "/../../../../tests/integration/uris"});
    p.waitForFinished();
    QVERIFY(p.exitStatus() == QProcess::NormalExit);
    QVERIFY(p.exitCode() == 0);
}

void tst_MediaUris::cleanupTestCase()
{
    QProcess p;
    p.start("../../../../tests/integration/uris/remove_test_files.sh");
    p.waitForFinished();
    QVERIFY(p.exitStatus() == QProcess::NormalExit);
    QVERIFY(p.exitCode() == 0);
}

void tst_MediaUris::init()
{
    // NOTE: This sleep is currently needed in order to give media-hub a bit of time
    // between our different tests to cleanup and come back in a state where it can
    // respond to our requests.
    sleep(1);
}

void tst_MediaUris::verifySpecialAsciiCharsCanPlay()
{
    QDir generatedFilesDir("/tmp/qtubuntu-media");
    QStringList mediaFiles = generatedFilesDir.entryList(QDir::NoDotAndDotDot | QDir::System
           | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

    qDebug() << "mediaFiles.size():" << mediaFiles.size();

    QMediaPlayer *player = new QMediaPlayer;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    player->setPlaylist(playlist);

    for (const QString &file : mediaFiles)
    {
        QString fullFilePath {"file://" + generatedFilesDir.absolutePath() + "/" + file};
        qDebug() << "Trying to play file:" << fullFilePath;
        playlist->addMedia(QUrl(fullFilePath));

        player->play();
        sleep(1);
        qDebug() << "player->error():" << player->error();
        qDebug() << "player->mediaStatus():" << player->mediaStatus();
        QVERIFY(player->mediaStatus() != QMediaPlayer::InvalidMedia);
        QCOMPARE(player->state(), QMediaPlayer::State::PlayingState);
        player->stop();
        playlist->clear();
    }

    delete playlist;
    delete player;
}

QTEST_GUILESS_MAIN(tst_MediaUris)
