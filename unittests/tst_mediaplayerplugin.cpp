/*
 * Copyright (C) 2013 Canonical, Ltd.
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
#include "service.h"
#include "aalmediaplayerservice.h"

#include <core/media/player.h>

#include <memory>

#include <QObject>
#include <QMediaPlayerControl>
#include <QVideoRendererControl>
#include <QtTest/QtTest>

#define private public

#include "aalmediaplayercontrol.h"

using namespace std;
using namespace core::ubuntu::media;

class tst_MediaPlayerPlugin : public QObject
{
    Q_OBJECT

    shared_ptr<AalMediaPlayerService> m_service;
    AalMediaPlayerControl *m_mediaPlayerControl;
    QMediaControl *m_playerControl;
    QMediaControl *m_rendererControl;
    shared_ptr<Player> m_player;
    shared_ptr<Service> m_hubService;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void tst_requestRelease();
    void tst_newMediaPlayer();
    void tst_setMedia();
    void tst_unescape();
    void tst_play();
    void tst_pause();
    void tst_stop();
    void tst_position();
    void tst_duration();
    void tst_isAudioSource();
    void tst_isVideoSource();
    void tst_volume();
};

void tst_MediaPlayerPlugin::initTestCase()
{
    m_hubService.reset(new TestService());
    m_service.reset(new AalMediaPlayerService(this));
    m_service->setService(m_hubService);    
    m_player.reset(new TestPlayer());
    m_service->setPlayer(m_player);
    m_mediaPlayerControl = m_service->mediaPlayerControl();
    m_playerControl = m_service->requestControl(QMediaPlayerControl_iid);
    QVERIFY(m_playerControl != NULL);
    m_rendererControl = m_service->requestControl(QVideoRendererControl_iid);
    QVERIFY(m_rendererControl != NULL);
}

void tst_MediaPlayerPlugin::cleanupTestCase()
{
    m_service->releaseControl(m_playerControl);
    m_service->releaseControl(m_rendererControl);
}

void tst_MediaPlayerPlugin::tst_requestRelease()
{
    QMediaControl *mpControl = NULL;
    QMediaControl *rendererControl = NULL;

    // Request a new reference to the media player control and verify that
    // it's the same pointer as the original reference.
    mpControl = m_service->requestControl(QMediaPlayerControl_iid);
    QVERIFY(mpControl == m_playerControl);
    rendererControl = m_service->requestControl(QVideoRendererControl_iid);
    QVERIFY(rendererControl == m_rendererControl);

    // Now release the reference and make sure that the original media player
    // control reference still exists.
    m_service->releaseControl(mpControl);
    QVERIFY(mpControl == m_playerControl);
    m_service->releaseControl(rendererControl);
    QVERIFY(rendererControl == m_rendererControl);
}

void tst_MediaPlayerPlugin::tst_newMediaPlayer()
{
    bool ret = m_service->newMediaPlayer();
    QVERIFY(ret == true);
}

void tst_MediaPlayerPlugin::tst_setMedia()
{
    QUrl url("blah.mp4");
    QMediaContent media(url);
    m_mediaPlayerControl->setMedia(media, NULL);
    QVERIFY(m_mediaPlayerControl->mediaStatus() == QMediaPlayer::LoadingMedia);
    QVERIFY(m_mediaPlayerControl->media() == media);
}

void tst_MediaPlayerPlugin::tst_unescape()
{
    QString uri_str("file:///home/phablet/Videos/sintel[REC].mp4");
    QUrl uri("file:///home/phablet/Videos/sintel%5BREC%5D.mp4");
    QVERIFY(m_mediaPlayerControl->unescape(QMediaContent(uri)).toString() == uri_str);

    uri_str = "https://www.youtube.com/watch?v=ESua4zGyo2Y&webm=1";
    uri = "https://www.youtube.com/watch?v=ESua4zGyo2Y&webm=1";
    QVERIFY(m_mediaPlayerControl->unescape(QMediaContent(uri)).toString() == uri_str);
}

void tst_MediaPlayerPlugin::tst_play()
{
    m_mediaPlayerControl->play();
    QVERIFY(m_mediaPlayerControl->mediaStatus() == QMediaPlayer::LoadedMedia);
}

void tst_MediaPlayerPlugin::tst_pause()
{
    m_mediaPlayerControl->pause();
    QVERIFY(m_mediaPlayerControl->state() == QMediaPlayer::PausedState);
}

void tst_MediaPlayerPlugin::tst_stop()
{
    m_mediaPlayerControl->stop();
    QVERIFY(m_mediaPlayerControl->state() == QMediaPlayer::StoppedState);
}

void tst_MediaPlayerPlugin::tst_position()
{
    m_mediaPlayerControl->setPosition(1e6);
    qDebug() << "position: " << m_mediaPlayerControl->position();
    QVERIFY(m_mediaPlayerControl->position() == 1e3);
}

void tst_MediaPlayerPlugin::tst_duration()
{
    QVERIFY(m_mediaPlayerControl->duration() == 1);
}

void tst_MediaPlayerPlugin::tst_isAudioSource()
{
    QVERIFY(m_mediaPlayerControl->isAudioAvailable());
}

void tst_MediaPlayerPlugin::tst_isVideoSource()
{
    QVERIFY(m_mediaPlayerControl->isVideoAvailable());
}

void tst_MediaPlayerPlugin::tst_volume()
{
    QVERIFY(m_mediaPlayerControl->volume() == 1);
}

QTEST_GUILESS_MAIN(tst_MediaPlayerPlugin)

#include "tst_mediaplayerplugin.moc"
