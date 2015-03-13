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
#include "aalmetadatareadercontrol.h"

#include <QMediaMetaData>
#include <QObject>
#include <QtTest/QtTest>

using namespace core::ubuntu::media;

void tst_MetaDataReaderControl::initTestCase()
{
    m_mdControlVideo = new AalMetaDataReaderControl(this);
    m_mdControlAudio = new AalMetaDataReaderControl(this);
    m_mdControlImage = new AalMetaDataReaderControl(this);

    QVERIFY(m_mdControlVideo != nullptr);
    QVERIFY(m_mdControlAudio != nullptr);
    QVERIFY(m_mdControlImage != nullptr);

    connect(this, SIGNAL(onMediaChanged(const QMediaContent&)), m_mdControlVideo, SLOT(onMediaChanged(const QMediaContent&)));
    connect(this, SIGNAL(onMediaChanged(const QMediaContent&)), m_mdControlAudio, SLOT(onMediaChanged(const QMediaContent&)));
    connect(this, SIGNAL(onMediaChanged(const QMediaContent&)), m_mdControlImage, SLOT(onMediaChanged(const QMediaContent&)));
}

void tst_MetaDataReaderControl::cleanupTestCase()
{
    delete m_mdControlImage;
    delete m_mdControlAudio;
    delete m_mdControlVideo;
}

// Adapted to have custom delta from qFuzzyCompare
static inline bool fuzzyCompare(double p1, double p2)
{
    return (qAbs(p1 - p2) <= 0.00001 * qMin(qAbs(p1), qAbs(p2)));
}

void tst_MetaDataReaderControl::setVideoMediaResource()
{
    m_mediaResource = QMediaResource(QUrl("/home/phablet/Videos/testfile.mp4"));
    m_media = QMediaContent(m_mediaResource);
    // Signal to the AalMetaDataReaderControl instance that we are setting example media to extract metadata from
    Q_EMIT onMediaChanged(m_media);
}

void tst_MetaDataReaderControl::setAudioMediaResource()
{
    m_mediaResource = QMediaResource(QUrl("/home/phablet/Music/testfile.ogg"));
    m_media = QMediaContent(m_mediaResource);
    // Signal to the AalMetaDataReaderControl instance that we are setting example media to extract metadata from
    Q_EMIT onMediaChanged(m_media);
}

void tst_MetaDataReaderControl::setImageMediaResource()
{
    m_mediaResource = QMediaResource(QUrl("/home/phablet/Pictures/testfile.jpg"));
    m_media = QMediaContent(m_mediaResource);
    // Signal to the AalMetaDataReaderControl instance that we are setting example media to extract metadata from
    Q_EMIT onMediaChanged(m_media);
}

void tst_MetaDataReaderControl::verifyResolution()
{
    setVideoMediaResource();
    QSize resolutionSize = m_mdControlVideo->metaData(QMediaMetaData::Resolution).value<QSize>();
    QCOMPARE(resolutionSize, QSize(320, 240));

    setImageMediaResource();
    resolutionSize = m_mdControlVideo->metaData(QMediaMetaData::Resolution).value<QSize>();
    QCOMPARE(resolutionSize, QSize(2448, 3264));
}

void tst_MetaDataReaderControl::verifyDuration()
{
    setVideoMediaResource();
    qint64 duration = m_mdControlVideo->metaData(QMediaMetaData::Duration).value<qint64>();
    QCOMPARE(duration, 34);

    setAudioMediaResource();
    duration = m_mdControlVideo->metaData(QMediaMetaData::Duration).value<qint64>();
    QCOMPARE(duration, 5);
}

void tst_MetaDataReaderControl::verifyTitle()
{
    setVideoMediaResource();
    QString title = m_mdControlVideo->metaData(QMediaMetaData::Title).value<QString>();
    QCOMPARE(title, QString("h264 title"));

    setAudioMediaResource();
    title = m_mdControlVideo->metaData(QMediaMetaData::Title).value<QString>();
    QCOMPARE(title, QString("track1"));
}

void tst_MetaDataReaderControl::verifyAuthor()
{
    setVideoMediaResource();
    QString author = m_mdControlVideo->metaData(QMediaMetaData::Author).value<QString>();
    QCOMPARE(author, QString("Canonical"));
}

void tst_MetaDataReaderControl::verifyGenre()
{
    setVideoMediaResource();
    QString genre = m_mdControlVideo->metaData(QMediaMetaData::Genre).value<QString>();
    QCOMPARE(genre, QString("Trailer"));

    setAudioMediaResource();
    genre = m_mdControlAudio->metaData(QMediaMetaData::Genre).value<QString>();
    QCOMPARE(genre, QString("Progressive Rock"));
}

void tst_MetaDataReaderControl::verifyDate()
{
    setAudioMediaResource();
    QDate date = m_mdControlAudio->metaData(QMediaMetaData::Date).value<QDate>();
    QCOMPARE(date, QDate(2013, 0, 0));

    setImageMediaResource();
    date = m_mdControlAudio->metaData(QMediaMetaData::Date).value<QDate>();
    QCOMPARE(date, QDate(2015, 2, 10));
}

void tst_MetaDataReaderControl::verifyAlbumTitle()
{
    setAudioMediaResource();
    QString title = m_mdControlAudio->metaData(QMediaMetaData::AlbumTitle).value<QString>();
    QCOMPARE(title, QString("album1"));
}

void tst_MetaDataReaderControl::verifyAlbumArtist()
{
    setAudioMediaResource();
    QString artist = m_mdControlAudio->metaData(QMediaMetaData::AlbumArtist).value<QString>();
    QCOMPARE(artist, QString("artist1"));
}

void tst_MetaDataReaderControl::verifyLatitude()
{
    setImageMediaResource();
    double latitude = m_mdControlImage->metaData(QMediaMetaData::GPSLatitude).value<double>();
    qDebug() << "latitude: " << latitude;
    QVERIFY(fuzzyCompare(latitude, 39.8566));
}

void tst_MetaDataReaderControl::verifyLongitude()
{
    setImageMediaResource();
    double longitude = m_mdControlImage->metaData(QMediaMetaData::GPSLongitude).value<double>();
    qDebug() << "longitude: " << longitude;
    QVERIFY(fuzzyCompare(longitude, -86.1242));
}
