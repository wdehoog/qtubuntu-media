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

#include "aalmediaplaylistcontrol.h"
#include "aalmediaplaylistprovider.h"

#include <core/signal.h>

#include <QGuiApplication>
#include <QMediaPlaylist>

#include <QDebug>

namespace media = core::ubuntu::media;

QT_BEGIN_NAMESPACE

namespace {
core::Signal<void> the_void;
}

AalMediaPlaylistControl::AalMediaPlaylistControl(QObject *parent)
    : QMediaPlaylistControl(parent),
      m_playlistProvider(nullptr),
      m_currentIndex(-1),
      m_trackChangedConnection(the_void.connect([](){}))
{
    qDebug() << Q_FUNC_INFO;
    connect(qGuiApp, &QGuiApplication::applicationStateChanged, this, &AalMediaPlaylistControl::onApplicationStateChanged);
}

AalMediaPlaylistControl::~AalMediaPlaylistControl()
{
    disconnect_signals();
}

QMediaPlaylistProvider* AalMediaPlaylistControl::playlistProvider() const
{
    return m_playlistProvider;
}

bool AalMediaPlaylistControl::setPlaylistProvider(QMediaPlaylistProvider *playlist)
{
    m_playlistProvider = playlist;
    return true;
}

int AalMediaPlaylistControl::currentIndex() const
{
    return m_currentIndex;
}

void AalMediaPlaylistControl::setCurrentIndex(int position)
{
    m_currentIndex = position;
}

int AalMediaPlaylistControl::nextIndex(int steps) const
{
    // algo: |(currentIndex + steps) - tracklist_size|
    return std::abs((m_currentIndex + steps) - m_playlistProvider->mediaCount());
}

int AalMediaPlaylistControl::previousIndex(int steps) const
{
    (void) steps;
    return 0;
}

void AalMediaPlaylistControl::next()
{
    qDebug() << Q_FUNC_INFO;

    try {
        m_hubPlayerSession->next();
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to go to next track: " << e.what();
    }
}

void AalMediaPlaylistControl::previous()
{
    qDebug() << Q_FUNC_INFO;

    try {
        m_hubPlayerSession->previous();
    }
    catch (const std::runtime_error &e) {
        qWarning() << "Failed to go to previous track: " << e.what();
    }
}

QMediaPlaylist::PlaybackMode AalMediaPlaylistControl::playbackMode() const
{
    return QMediaPlaylist::Sequential;
}

void AalMediaPlaylistControl::setPlaybackMode(QMediaPlaylist::PlaybackMode mode)
{
    (void) mode;
}

void AalMediaPlaylistControl::setPlayerSession(const std::shared_ptr<core::ubuntu::media::Player>& playerSession)
{
    m_hubPlayerSession = playerSession;
    aalMediaPlaylistProvider()->setPlayerSession(playerSession);

    try {
        m_hubTrackList = m_hubPlayerSession->track_list();
    }
    catch (std::runtime_error &e) {
        qWarning() << "FATAL: Failed to retrieve the current player session TrackList: " << e.what();
    }

    connect_signals();
}

void AalMediaPlaylistControl::onApplicationStateChanged(Qt::ApplicationState state)
{
    qDebug() << "*** " << Q_FUNC_INFO;

    switch (state)
    {
    case Qt::ApplicationSuspended:
        qDebug() << "**Application has been suspended";
        break;
    case Qt::ApplicationActive:
        qDebug() << "**Application is now active";
        break;
    default:
        qDebug() << "Unknown ApplicationState or purposefully not reacting to current state change";
        break;
    }
}

void AalMediaPlaylistControl::connect_signals()
{
    qDebug() << Q_FUNC_INFO;

    m_trackChangedConnection = m_hubTrackList->on_track_changed().connect([this](const media::Track::Id& id)
    {
        qDebug() << "onTrackChanged, id: " << id.c_str();
        if (!id.empty())
        {
            m_currentIndex = aalMediaPlaylistProvider()->indexOfTrack(id);
            qDebug() << "m_currentIndex updated to: " << m_currentIndex;
        }
    });
}

void AalMediaPlaylistControl::disconnect_signals()
{
    qDebug() << Q_FUNC_INFO;

    if (m_trackChangedConnection.is_connected())
        m_trackChangedConnection.disconnect();
}

AalMediaPlaylistProvider* AalMediaPlaylistControl::aalMediaPlaylistProvider()
{
    return static_cast<AalMediaPlaylistProvider*>(m_playlistProvider);
}

QT_END_NAMESPACE
