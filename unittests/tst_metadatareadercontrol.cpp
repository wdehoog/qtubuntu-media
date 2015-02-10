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

#if 0
    m_hubService.reset(new TestService());
    m_hubPlayer.reset(new TestPlayer());
    qDebug() << "Creating new AalMediaPlayerService(this)";
    m_service = new AalMediaPlayerService(m_hubService, m_hubPlayer, this);
#endif
    //qDebug() << "Calling setService with m_hubService";
    //m_service->setService(m_hubService);
    //m_service->setPlayer(m_player);
    m_qMediaPlayer = new QMediaPlayer(this);

    //m_playerControl = m_service->requestControl(QMediaPlayerControl_iid);
#if 0
    m_mediaPlayerControl = m_service->mediaPlayerControl();
#endif
    //QVERIFY(m_playerControl != NULL);
}

void tst_MetaDataReaderControl::getResolution()
{
#if 0
    bool ret = m_service->newMediaPlayer();
    QVERIFY(ret == true);
#endif
    //QMediaResource mediaResource(QUrl("/home/phablet/Videos/h264.avi"));
    QMediaResource mediaResource(QUrl("/home/phablet/Videos/com.ubuntu.camera/video20150113_161726582.mp4"));
    QMediaContent media(mediaResource);
    if (!m_qMediaPlayer->isAvailable())
    {
        qWarning() << "QMediaService is not available.";
        Q_ASSERT(false);
    }
    qDebug() << "Setting media source file on QMediaPlayer instance";
    m_qMediaPlayer->setMedia(media);
#if 0
    m_mediaPlayerControl->setMedia(media, nullptr);
#endif

    qDebug() << "Requesting QMediaMetaData";
    QSize resolutionSize = m_qMediaPlayer->metaData(QMediaMetaData::Resolution).value<QSize>();
    qDebug() << "Requested QMediaMetaData";
    qDebug() << "m_qMediaPlayer->isMetaDataAvailable(): " << m_qMediaPlayer->isMetaDataAvailable();
    qDebug() << "resolutionSize: " << resolutionSize;
}
