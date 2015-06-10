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

#include <QDebug>

namespace media = core::ubuntu::media;

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

    try {
        return m_hubTrackList->tracks()->size();
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to get playlist media count: " << e.what();
        return 0;
    }
}

QMediaContent AalMediaPlaylistProvider::media(int index) const
{
    (void) index;
    return QMediaContent();
}

bool AalMediaPlaylistProvider::isReadOnly() const
{
    return m_hubTrackList->can_edit_tracks();
}

bool AalMediaPlaylistProvider::addMedia(const QMediaContent &content)
{
    qDebug() << Q_FUNC_INFO;

    try {
        static const bool make_current = false;
        const QUrl url = content.canonicalUrl();
        std::string urlStr = AalUtility::unescape_str(content);
        qDebug() << "scheme: " << url.scheme();
        qDebug() << "Contains file://?" << url.toString().contains("file://", Qt::CaseInsensitive);
        if (url.scheme().isEmpty() and url.scheme() != "file")
            urlStr = "file://" + AalUtility::unescape_str(content);

        m_hubTrackList->add_track_with_uri_at(urlStr, media::TrackList::after_empty_track(), make_current);
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to add track '" << content.canonicalUrl().toString() << "' to playlist: " << e.what();
        return false;
    }

    return true;
}

bool AalMediaPlaylistProvider::addMedia(const QList<QMediaContent> &contentList)
{
    qDebug() << Q_FUNC_INFO;

    if (contentList.empty())
        return false;

    for (int i=0; i<contentList.count(); i++)
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

    return false;
}

bool AalMediaPlaylistProvider::insertMedia(int index, const QList<QMediaContent> &content)
{
    (void) index;
    (void) content;

    return false;
}

bool AalMediaPlaylistProvider::removeMedia(int pos)
{
    (void) pos;

    return false;
}

bool AalMediaPlaylistProvider::removeMedia(int start, int end)
{
    (void) start;
    (void) end;

    return false;
}

bool AalMediaPlaylistProvider::clear()
{
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
}

QT_END_NAMESPACE
