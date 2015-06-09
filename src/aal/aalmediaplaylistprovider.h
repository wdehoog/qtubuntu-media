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

#ifndef AALMEDIAPLAYLISTPROVIDER_H
#define AALMEDIAPLAYLISTPROVIDER_H

#include <private/qmediaplaylistprovider_p.h>

#include <core/media/player.h>
#include <core/media/track_list.h>

#include <memory>

QT_BEGIN_NAMESPACE

class AalMediaPlaylistProvider : public QMediaPlaylistProvider
{
Q_OBJECT
public:
    AalMediaPlaylistProvider(QObject *parent=0);
    ~AalMediaPlaylistProvider();

    int mediaCount() const;
    QMediaContent media(int index) const;

    bool isReadOnly() const;

    bool addMedia(const QMediaContent &content);

    void setPlayerSession(const std::shared_ptr<core::ubuntu::media::Player>& playerSession);

private:
    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayerSession;
    std::shared_ptr<core::ubuntu::media::TrackList> m_hubTrackList;
};

QT_END_NAMESPACE

#endif
