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
    return 0;
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

    static const bool make_current = false;
    m_hubTrackList->add_track_with_uri_at(AalUtility::unescape_str(content), media::TrackList::after_empty_track(), make_current);

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
