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

#include "aalmediaplayerserviceplugin.h"
#include "aalmediaplayerservice.h"

#include <QDebug>

AalServicePlugin::AalServicePlugin()
{
    qDebug() << "Initializing AalServicePlugin";
}

QMediaService* AalServicePlugin::create(QString const& key)
{
    qDebug() << Q_FUNC_INFO << key;

    if (key == QLatin1String(Q_MEDIASERVICE_MEDIAPLAYER))
        return new AalMediaPlayerService();

    return nullptr;
}

void AalServicePlugin::release(QMediaService *service)
{
    delete service;
}

QList<QByteArray> AalServicePlugin::devices(const QByteArray &service) const
{
    Q_UNUSED(service);
    return QList<QByteArray>();
}

QString AalServicePlugin::deviceDescription(const QByteArray &service, const QByteArray &device)
{
    Q_UNUSED(service);
    Q_UNUSED(device);
    return QString();
}
