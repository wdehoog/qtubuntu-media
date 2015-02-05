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

#ifndef AALMETADATAREADERCONTROL_H
#define AALMETADATAREADERCONTROL_H

#include <QMetaDataReaderControl>
#include <qmediacontent.h>

QT_BEGIN_NAMESPACE

class AalMetaDataReaderControl : public QMetaDataReaderControl
{
    Q_OBJECT
public:
    explicit AalMetaDataReaderControl(QObject *parent = 0);

    bool isMetaDataAvailable() const Q_DECL_OVERRIDE;

    QVariant metaData(const QString &key) const Q_DECL_OVERRIDE;
    QStringList availableMetaData() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void onMediaChanged(const QMediaContent &media);
    void onUpdateMetaData();

private:
    void updateMetaData();

    QMediaContent m_mediaContent;
    bool m_available;
    QVariantMap m_metadata;
};

QT_END_NAMESPACE

#endif // AALMETADATAREADERCONTROL_H
