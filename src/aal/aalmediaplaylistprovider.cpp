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
#include "aalutility.h"

#include <string>
#include <sstream>

#include <QDebug>

namespace media = core::ubuntu::media;

Q_DECLARE_METATYPE(core::ubuntu::media::Track::Id)

QT_BEGIN_NAMESPACE

namespace {
core::Signal<void> the_void;
}

AalMediaPlaylistProvider::AalMediaPlaylistProvider(QObject *parent)
    : QMediaPlaylistProvider(parent),
      m_trackAddedConnection(the_void.connect([](){})),
      m_trackRemovedConnection(the_void.connect([](){}))
{
    qDebug() << Q_FUNC_INFO;
    qRegisterMetaType<core::ubuntu::media::Track::Id>();
}

AalMediaPlaylistProvider::~AalMediaPlaylistProvider()
{
    qDebug() << Q_FUNC_INFO;
    disconnect_signals();
}

int AalMediaPlaylistProvider::mediaCount() const
{
    if (!m_hubTrackList) {
        qWarning() << "Tracklist doesn't exist";
        return 0;
    }

    return track_index_lut.size();
}

QMediaContent AalMediaPlaylistProvider::media(int index) const
{
    if (!m_hubTrackList || mediaCount() == 0)
        return QMediaContent();

    const media::Track::Id id = trackOfIndex(index);

    QMediaContent content;
    try {
        media::Track::UriType uri = m_hubTrackList->query_uri_for_track(id);
        content = QMediaContent(QUrl(QString(uri.c_str())));
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to query track for URI: " << e.what();
        return QMediaContent();
    }

    return content;
}

bool AalMediaPlaylistProvider::isReadOnly() const
{
    if (!m_hubTrackList) {
        qWarning() << "Track list does not exist!";
        return false;
    }

    try {
        return m_hubTrackList->can_edit_tracks();
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to see if tracklist is editable: " << e.what();
        return false;
    }
}

bool AalMediaPlaylistProvider::addMedia(const QMediaContent &content)
{
    if (!m_hubTrackList) {
        qWarning() << "Track list does not exist so can't add a new track!";
        return false;
    }

    const QUrl url = content.canonicalUrl();
    std::string urlStr = AalUtility::unescape_str(content);
    if (url.scheme().isEmpty() and url.scheme() != "file")
        urlStr = "file://" + urlStr;

    static const bool make_current = false;
    const media::Track::Id after_empty_track = media::TrackList::after_empty_track();

    const int newIndex = track_index_lut.size();
    Q_EMIT mediaAboutToBeInserted(newIndex, newIndex);
    try {
        m_hubTrackList->add_track_with_uri_at(urlStr, after_empty_track, make_current);
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to add track '" << content.canonicalUrl().toString() << "' to playlist: " << e.what();
        return false;
    }

    return true;
}

bool AalMediaPlaylistProvider::addMedia(const QList<QMediaContent> &contentList)
{
    if (contentList.empty())
        return false;

    uint16_t i;
    for (i=0; i<contentList.count(); i++)
    {
        if (!addMedia(contentList.at(i)))
        {
            qWarning() << "Not all tracks were added to the playlist";
            return false;
        }
    }

    return true;
}

bool AalMediaPlaylistProvider::insertMedia(int index, const QMediaContent &content)
{
    (void) index;
    (void) content;

    qWarning() << Q_FUNC_INFO << " - Not yet implemented";

    return false;
}

bool AalMediaPlaylistProvider::insertMedia(int index, const QList<QMediaContent> &content)
{
    (void) index;
    (void) content;

    qWarning() << Q_FUNC_INFO << " - Not yet implemented";

    return false;
}

bool AalMediaPlaylistProvider::removeMedia(int pos)
{
    if (!m_hubTrackList) {
        qWarning() << "Track list does not exist so can't remove track!";
        return false;
    }

    const media::Track::Id id = trackOfIndex(pos);
    if (id.empty())
        return false;

    try {
        m_hubTrackList->remove_track(id);
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to remove track index " << pos << ": " << e.what();
        return false;
    }

    return true;
}

bool AalMediaPlaylistProvider::removeMedia(int start, int end)
{
    for (int i=start; i<=end; i++)
    {
        if (!removeMedia(i))
        {
            qWarning() << "Failed to remove the full range of tracks requested";
            return false;
        }
    }

    return true;
}

bool AalMediaPlaylistProvider::clear()
{
    if (!m_hubTrackList) {
        qWarning() << "Track list doesn't exist so can't clear it!";
        return false;
    }

    try {
        m_hubTrackList->reset();
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to clear the playlist: " << e.what();
        return false;
    }

    return true;
}

void AalMediaPlaylistProvider::setPlayerSession(const std::shared_ptr<core::ubuntu::media::Player>& playerSession)
{
    m_hubPlayerSession = playerSession;

    try {
        m_hubTrackList = m_hubPlayerSession->track_list();
    }
    catch (std::runtime_error &e) {
        qWarning() << "FATAL: Failed to retrieve the current player session TrackList: " << e.what();
    }

    connect_signals();
}

void AalMediaPlaylistProvider::connect_signals()
{
    if (!m_hubTrackList) {
        qWarning() << "Can't connect to track list signals as it doesn't exist";
        return;
    }

    m_trackAddedConnection = m_hubTrackList->on_track_added().connect([this](const media::Track::Id& id)
    {
        track_index_lut.push_back(id);
        // This must come after push_back(id) or indexOfTrack won't find id in the LUT
        const int index = indexOfTrack(id);
        Q_EMIT mediaAboutToBeInserted(index, index);
        qDebug() << "mediaInserted, index: " << index;
        Q_EMIT mediaInserted(index, index);
    });

    m_trackRemovedConnection = m_hubTrackList->on_track_removed().connect([this](const media::Track::Id& id)
    {
        const int index = indexOfTrack(id);
        Q_EMIT mediaAboutToBeRemoved(index, index);
        // Remove the track from the local look-up-table
        const bool ret = removeTrack(id);
        if (!ret)
            qWarning() << "Failed to remove track with id " << id.c_str() << " from track_index_lut";

        // Removed one track, so start and end are the same index values
        Q_EMIT mediaRemoved(index, index);
    });
}

void AalMediaPlaylistProvider::disconnect_signals()
{
    qDebug() << Q_FUNC_INFO;

    if (m_trackRemovedConnection.is_connected())
        m_trackRemovedConnection.disconnect();

    if (m_trackAddedConnection.is_connected())
        m_trackAddedConnection.disconnect();
}

bool AalMediaPlaylistProvider::removeTrack(const core::ubuntu::media::Track::Id &id)
{
    if (id.empty())
        return false;

    const int pos = indexOfTrack(id);
    if (pos == -1)
        return false;

    qDebug() << "Erasing track from track lookup table: " << id.c_str();
    track_index_lut.erase(track_index_lut.begin() + pos);

    return true;
}

int AalMediaPlaylistProvider::indexOfTrack(const media::Track::Id &id) const
{
    if (id.empty() || track_index_lut.size() == 0)
        return -1;

    const auto trackPos = std::find(track_index_lut.begin(), track_index_lut.end(), id);
    if (trackPos == track_index_lut.end())
    {
        qWarning() << "Failed to look up index for non-existent track id: " << id.c_str();
        return -1;
    }

    return std::distance(track_index_lut.begin(), trackPos);
}

const media::Track::Id AalMediaPlaylistProvider::trackOfIndex(int index) const
{
    if (track_index_lut.size() == 0)
    {
        qWarning() << "track_index_lut is empty, can't return valid track id for index: " << index;
        return media::TrackList::after_empty_track();
    }

    try {
        return track_index_lut.at(index);
    }
    catch (const std::out_of_range &e) {
        qWarning() << "Failed to look up track for index " << index <<
            "- out of range: track_index_lut.size() is " << track_index_lut.size();
        return media::TrackList::after_empty_track();
    }
}

QT_END_NAMESPACE
