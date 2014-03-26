/*
 * Copyright (C) 2014 Canonical, Ltd.
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
 *
 * 	Author: Ricardo Mendoza <ricardo.mendoza@canonical.com>
 */

#include "qubuntumedia.h"
#include <qqmlcomponent.h>
#include <qqmlextensionplugin.h>
#include <QQmlEngine>
#include <QQmlExtensionPlugin>

class QUbuntuMediaPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

    virtual void registerTypes(const char *uri)
    {
        ASSERT(QLatin1String(uri) == QLatin1String("Ubuntu.Media"));
        qmlRegisterType<QUbuntuMedia>(uri, 1, 0, "BackgroundMedia");
    }
};

#include "plugin.moc"
