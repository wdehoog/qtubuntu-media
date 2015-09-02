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

#include "aalutility.h"

#include <QDebug>

QUrl AalUtility::unescape(const QMediaContent &media)
{
    if (media.isNull())
        return QUrl();

    if (media.canonicalUrl().isLocalFile()) {
        qDebug() << "Local file URI: " << QUrl::fromPercentEncoding(media.canonicalUrl().toString().toUtf8());
        return QUrl::fromPercentEncoding(media.canonicalUrl().toString().toUtf8());
    }
    else {
        qDebug() << "Remote stream URI: " << QUrl::fromEncoded(media.canonicalUrl().toString().toUtf8());
        return QUrl::fromEncoded(media.canonicalUrl().toString().toUtf8());
    }
}

std::string AalUtility::unescape_str(const QMediaContent &media)
{
    return unescape(media).toString().toStdString();
}
