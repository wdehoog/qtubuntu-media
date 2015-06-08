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

#include "aalmediaplaylistprovider.h"

#include <QDebug>

QT_BEGIN_NAMESPACE

AalMediaPlaylistProvider::AalMediaPlaylistProvider(QObject *parent)
    : QMediaPlaylistProvider(parent)
{
    qDebug() << Q_FUNC_INFO;
}

AalMediaPlaylistProvider::~AalMediaPlaylistProvider()
{
}

int AalMediaPlaylistProvider::mediaCount() const
{
    qDebug() << Q_FUNC_INFO;
    return 0;
}

QMediaContent AalMediaPlaylistProvider::media(int index) const
{
    (void) index;
    return QMediaContent();
}

bool AalMediaPlaylistProvider::addMedia(const QMediaContent &content)
{
    qDebug() << Q_FUNC_INFO;
}

QT_END_NAMESPACE
