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
#include <core/media/track.h>
#include <core/media/track_list.h>

#include <core/connection.h>

#include <atomic>
#include <memory>
#include <vector>

QT_BEGIN_NAMESPACE

class AalMediaPlaylistControl;

class AalMediaPlaylistProvider : public QMediaPlaylistProvider
{
Q_OBJECT
public:
    friend class AalMediaPlaylistControl;

    AalMediaPlaylistProvider(QObject *parent=0);
    ~AalMediaPlaylistProvider();

    int mediaCount() const;
    QMediaContent media(int index) const;

    bool isReadOnly() const;

    bool addMedia(const QMediaContent &content);
    bool addMedia(const QList<QMediaContent> &contentList);
    bool insertMedia(int index, const QMediaContent &content);
    bool insertMedia(int index, const QList<QMediaContent> &content);
    bool removeMedia(int pos);
    bool removeMedia(int start, int end);
    bool clear();

private:
    void setPlayerSession(const std::shared_ptr<core::ubuntu::media::Player>& playerSession);
    void connect_signals();
    void disconnect_signals();
    bool removeTrack(const core::ubuntu::media::Track::Id &id);
    // Finds the index of the first tracks that matches id, or the last if reverse is true
    int indexOfTrack(const core::ubuntu::media::Track::Id &id, bool reverse=false) const;
    const core::ubuntu::media::Track::Id trackOfIndex(int index) const;

    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayerSession;
    std::shared_ptr<core::ubuntu::media::TrackList> m_hubTrackList;

    core::Connection m_trackAddedConnection;
    core::Connection m_tracksAddedConnection;
    core::Connection m_trackRemovedConnection;
    core::Connection m_trackListResetConnection;

    // Simple table that holds a list (order is significant and explicit) of
    // Track::Id's for a lookup. track_index_lut.at[x] gives the corresponding
    // Track::Id for index x, and vice-versa.
    std::vector<core::ubuntu::media::Track::Id> track_index_lut;

    // Did the client perform an insertTrack() (as opposed to an addTrack()) operation?
    // If yes, the index will be zero or greater, if not index will be -1;
    std::atomic<int> m_insertTrackIndex;
};

QT_END_NAMESPACE

#endif
