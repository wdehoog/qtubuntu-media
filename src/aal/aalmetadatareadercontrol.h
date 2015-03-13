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

class tst_MetaDataReaderControl;

QT_BEGIN_NAMESPACE

class AalMetaDataReaderControl : public QMetaDataReaderControl
{
    Q_OBJECT

    // For unit testing purposes
    friend class tst_MetaDataReaderControl;

public:
    explicit AalMetaDataReaderControl(QObject *parent = 0);

    /**
     * @brief Returns whether there is any metadata available to read for the given media source.
     */
    bool isMetaDataAvailable() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns a metadata value given a metadata key name. See QMediaMetaData for a list of keys.
     * @return Returns a QVariant with the metadata value.
     */
    QVariant metaData(const QString &key) const Q_DECL_OVERRIDE;
    /**
     * @brief Returns a list of key names for which metadata values are available.
     * @return Returns a QStringList of key names.
     */
    QStringList availableMetaData() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void onMediaChanged(const QMediaContent &media);
    /**
     * @brief Updates the internal metadata metadata store from the mediascanner for the given media source.
     **/
    void onUpdateMetaData();

private:
    void updateMetaData();

    QMediaContent m_mediaContent;
    bool m_available;
    QVariantMap m_metadata;
};

QT_END_NAMESPACE

#endif // AALMETADATAREADERCONTROL_H
