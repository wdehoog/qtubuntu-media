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

#include <QMediaContent>
#include <QMediaPlayer>
#include <QMediaResource>
#include <QMediaMetaData>
#include <QObject>
#include <QtTest/QtTest>

void tst_MetaDataReaderControl::initTestCase()
{
    qDebug() << Q_FUNC_INFO;
}

void tst_MetaDataReaderControl::getResolution()
{
    QMediaPlayer p;
    QMediaResource mediaResource(QUrl("file://home/phablet/Videos/h264.avi"));
    QMediaContent media(mediaResource);
    qDebug() << "Setting media source file on QMediaPlayer instance";
    p.setMedia(media);

    qDebug() << "Requesting QMediaMetaData";
    QSize resolutionSize = p.metaData(QMediaMetaData::Resolution).value<QSize>();
    qDebug() << "Requested QMediaMetaData";
    qDebug() << "resolutionSize: " << resolutionSize;
}
