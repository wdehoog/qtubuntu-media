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

#include "tst_metadatareadercontrol.h"
#include "player.h"
#include "service.h"
#include "aalmediaplayerservice.h"
#include "aalmetadatareadercontrol.h"

#include <QMediaContent>
#include <QMediaControl>
#include <QMediaPlayer>
#include <QMediaResource>
#include <QMediaMetaData>
#include <QObject>
#include <QtTest/QtTest>

#define private public
#include "aalmediaplayercontrol.h"

using namespace core::ubuntu::media;

void tst_MetaDataReaderControl::initTestCase()
{
    qDebug() << Q_FUNC_INFO;

    m_hubService.reset(new TestService());
    m_hubPlayer.reset(new TestPlayer());
    qDebug() << "Creating new AalMediaPlayerService(this)";
    m_service = new AalMediaPlayerService(m_hubService, m_hubPlayer, this);
    //qDebug() << "Calling setService with m_hubService";
    //m_service->setService(m_hubService);
    //m_service->setPlayer(m_player);
    m_qMediaPlayer = new QMediaPlayer(this);

    //m_playerControl = m_service->requestControl(QMediaPlayerControl_iid);
    m_mediaPlayerControl = m_service->mediaPlayerControl();
    //QVERIFY(m_playerControl != NULL);
}

void tst_MetaDataReaderControl::getResolution()
{
    bool ret = m_service->newMediaPlayer();
    QVERIFY(ret == true);
    //QMediaResource mediaResource(QUrl("/home/phablet/Videos/h264.avi"));
    QMediaResource mediaResource(QUrl("/home/phablet/Videos/com.ubuntu.camera/video20150113_161726582.mp4"));
    QMediaContent media(mediaResource);
    qDebug() << "Setting media source file on QMediaPlayer instance";
    m_qMediaPlayer->setMedia(media);
    m_mediaPlayerControl->setMedia(media, nullptr);

    qDebug() << "Requesting QMediaMetaData";
    QSize resolutionSize = m_qMediaPlayer->metaData(QMediaMetaData::Resolution).value<QSize>();
    qDebug() << "Requested QMediaMetaData";
    qDebug() << "m_qMediaPlayer->isMetaDataAvailable(): " << m_qMediaPlayer->isMetaDataAvailable();
    qDebug() << "resolutionSize: " << resolutionSize;
}
