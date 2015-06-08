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

#ifndef AALMEDIAPLAYLISTCONTROL_H
#define AALMEDIAPLAYLISTCONTROL_H

#include <private/qmediaplaylistcontrol_p.h>

#include <core/media/player.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QMediaPlaylistProvider;

class AalMediaPlaylistControl : public QMediaPlaylistControl
{
Q_OBJECT
public:
    AalMediaPlaylistControl(QObject *parent=0);
    ~AalMediaPlaylistControl();

    QMediaPlaylistProvider* playlistProvider() const;
    bool setPlaylistProvider(QMediaPlaylistProvider *playlist);

    int currentIndex() const;
    void setCurrentIndex(int position);
    int nextIndex(int steps) const;
    int previousIndex(int steps) const;

    void next();
    void previous();

    QMediaPlaylist::PlaybackMode playbackMode() const;
    void setPlaybackMode(QMediaPlaylist::PlaybackMode mode);

    void setPlayerSession(const std::shared_ptr<core::ubuntu::media::Player>& playerSession);

private Q_SLOTS:
    void onApplicationStateChanged(Qt::ApplicationState state);

private:
    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayerSession;
    QMediaPlaylistProvider *m_playlistProvider;

    int m_currentIndex;
};

QT_END_NAMESPACE

#endif
