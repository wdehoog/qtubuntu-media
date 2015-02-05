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
#include <mediascanner/MediaFileBuilder.hh>

#include <QDebug>

using namespace mediascanner;

AalMetaDataReaderControl::AalMetaDataReaderControl(QObject *parent)
    : QMetaDataReaderControl(parent),
      m_available(false)
{
    qDebug() << Q_FUNC_INFO;
}

bool AalMetaDataReaderControl::isMetaDataAvailable() const
{
    return m_available;
}

QVariant AalMetaDataReaderControl::metaData(const QString &key) const
{
    return m_metadata.value(key);
}

QStringList AalMetaDataReaderControl::availableMetaData() const
{
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

    MediaFileBuilder mfb(m_mediaContent.canonicalUrl().toString().toStdString());
    MediaFile mf(mfb.build());
}
