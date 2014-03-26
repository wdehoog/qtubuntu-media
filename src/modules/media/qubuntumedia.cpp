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

QUbuntuMedia::QUbuntuMedia(QObject* parent)
    : QObject(parent),
      m_player(nullptr),
      m_mediaPlaylist(new QMediaPlaylist())
{
}

void QUbuntuMedia::setPlaylist(const QList<QUrl> &playlist, int index, PlaybackMode mode)
{
    if (m_player == nullptr)
        return;

    if (m_qmlPlaylist == playlist)
        return;

    m_qmlPlaylist = playlist;

    // Wipe state
    if (!m_mediaPlaylist->isEmpty()) {
        m_mediaPlaylist->clear();
    }

    if (playlist.empty())
        return;

    Q_FOREACH(const QUrl& mediaUrl, playlist) {
        m_mediaPlaylist->addMedia(mediaUrl);
    }
    m_mediaPlaylist->setCurrentIndex(index);

    switch (mode)
    {
    case PlaybackMode::CurrentItemOnce: m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce); break;
    case PlaybackMode::CurrentItemInLoop: m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); break;
    case PlaybackMode::Sequential: m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::Sequential); break;
    case PlaybackMode::Loop: m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop); break;
    case PlaybackMode::Random: m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::Random); break;
    default:
        break;
    }
   
    // FIXME: This is a hack, however there is currently no other way to get a Playlist to
    // the media controls. Qt takes it upon itself to walk playlists, that breaks our
    // out-of-process scenario.
    m_player->setMedia(0, reinterpret_cast<QIODevice*>(m_mediaPlaylist));

    Q_EMIT playlistChanged();
}

void QUbuntuMedia::setMediaPlayer(QObject *mediaPlayer)
{
    if (mediaPlayer == m_player)
        return;

    m_player = qobject_cast<QMediaPlayer*>(mediaPlayer->property("mediaObject").value<QObject*>());
}
