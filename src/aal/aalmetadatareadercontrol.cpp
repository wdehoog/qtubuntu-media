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

#include "aalmetadatareadercontrol.h"

#include <mediascanner/MediaFile.hh>
#include <mediascanner/MediaStore.hh>

#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QMediaMetaData>
#include <QSize>

using namespace mediascanner;

AalMetaDataReaderControl::AalMetaDataReaderControl(QObject *parent)
    : QMetaDataReaderControl(parent),
      m_available(false)
{
    qDebug() << Q_FUNC_INFO;
}

bool AalMetaDataReaderControl::isMetaDataAvailable() const
{
    qDebug() << Q_FUNC_INFO;
    return m_available;
}

QVariant AalMetaDataReaderControl::metaData(const QString &key) const
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "key: " << key << ", value: " << m_metadata.value(key);
    qDebug() << "m_metadata: " << m_metadata;
    return m_metadata.value(key);
}

QStringList AalMetaDataReaderControl::availableMetaData() const
{
    qDebug() << Q_FUNC_INFO;
    return m_metadata.keys();
}

void AalMetaDataReaderControl::onMediaChanged(const QMediaContent &media)
{
    qDebug() << Q_FUNC_INFO;
    m_mediaContent = media;
    updateMetaData();
}

void AalMetaDataReaderControl::onUpdateMetaData()
{
    qDebug() << Q_FUNC_INFO;
    if (m_mediaContent.isNull())
        return;

    updateMetaData();
}

void AalMetaDataReaderControl::updateMetaData()
{
    qDebug() << Q_FUNC_INFO;
    if (m_mediaContent.isNull())
    {
        qWarning() << "Not updating metadata since the source has not been set.";
        return;
    }

    m_metadata.clear();

    bool isVideo = false, isAudio = false, isImage = false;
    try {
        MediaStore store(MS_READ_ONLY);
        qDebug() << "Doing a mediascanner lookup of file: " << m_mediaContent.canonicalUrl().toString(QUrl::RemoveScheme);
        MediaFile mf = store.lookup(m_mediaContent.canonicalUrl().toString(QUrl::RemoveScheme).toStdString());
        isVideo = (mf.getType() == VideoMedia);
        isAudio = (mf.getType() == AudioMedia);
        isImage = (mf.getType() == ImageMedia);
        m_metadata.insert(QMediaMetaData::Title, QString(mf.getTitle().c_str()));
        m_metadata.insert(QMediaMetaData::Author, QStringList(QString(mf.getAuthor().c_str())));
        {
            QString format = "yyyy-MM-ddThh:mm:ss";
            QDateTime datetime = QDateTime::fromString(QString(mf.getDate().c_str()), format);
            m_metadata.insert(QMediaMetaData::Date, datetime.date());
        }
        m_metadata.insert(QMediaMetaData::Genre, QStringList(QString(mf.getGenre().c_str())));
        m_metadata.insert(QMediaMetaData::Duration, static_cast<qint64>(mf.getDuration()));

        if (isVideo)
        {
            m_metadata.insert(QMediaMetaData::Resolution, QSize(mf.getWidth(), mf.getHeight()));
        }

        if (isAudio)
        {
            m_metadata.insert(QMediaMetaData::AlbumTitle, QString(mf.getAlbum().c_str()));
            m_metadata.insert(QMediaMetaData::AlbumArtist, QString(mf.getAlbumArtist().c_str()));
            m_metadata.insert(QMediaMetaData::CoverArtUrlSmall, QUrl(mf.getArtUri().c_str()));
            m_metadata.insert(QMediaMetaData::CoverArtUrlLarge, QUrl(mf.getArtUri().c_str()));
            m_metadata.insert(QMediaMetaData::TrackNumber, mf.getTrackNumber());
        }

        if (isImage)
        {
            if (m_metadata.value(QMediaMetaData::Resolution).value<QSize>().isEmpty())
                m_metadata.insert(QMediaMetaData::Resolution, QSize(mf.getWidth(), mf.getHeight()));

            m_metadata.insert(QMediaMetaData::GPSLatitude, mf.getLatitude());
            m_metadata.insert(QMediaMetaData::GPSLongitude, mf.getLongitude());
        }

        qDebug() << "-----------------------------------------------------";
        qDebug() << "Media type: " << ((isVideo) ? "video" : "audio");
        qDebug() << "Media title: " << mf.getTitle().c_str();
        qDebug() << "Media author: " << mf.getAuthor().c_str();
        qDebug() << "Media date: " << mf.getDate().c_str();
        qDebug() << "Media genre: " << mf.getGenre().c_str();
        qDebug() << "Media duration: " << mf.getDuration();
        qDebug() << "Media height: " << mf.getHeight();
        qDebug() << "Media width: " << mf.getWidth();
        qDebug() << "Latitude: " << mf.getLatitude();
        qDebug() << "Longitude: " << mf.getLongitude();
        qDebug() << "-----------------------------------------------------";
    } catch (std::runtime_error &e) {
        qWarning() << "Caught runtime exception: " << e.what();
    }

    bool previousAvailable = m_available;
    m_available = !m_metadata.isEmpty();

    if (m_available != previousAvailable)
        Q_EMIT metaDataAvailableChanged(m_available);

    Q_EMIT metaDataChanged();
}
