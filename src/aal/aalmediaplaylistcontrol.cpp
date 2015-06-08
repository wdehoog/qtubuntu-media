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

#include <QGuiApplication>
#include <QMediaPlaylist>

#include <QDebug>

QT_BEGIN_NAMESPACE

AalMediaPlaylistControl::AalMediaPlaylistControl(QObject *parent)
    : QMediaPlaylistControl(parent),
      m_playlistProvider(nullptr),
      m_currentIndex(-1)
{
    qDebug() << Q_FUNC_INFO;
    connect(qGuiApp, &QGuiApplication::applicationStateChanged, this, &AalMediaPlaylistControl::onApplicationStateChanged);
}

AalMediaPlaylistControl::~AalMediaPlaylistControl()
{
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
    (void) steps;
    return 0;
}

int AalMediaPlaylistControl::previousIndex(int steps) const
{
    (void) steps;
    return 0;
}

void AalMediaPlaylistControl::next()
{
    qDebug() << Q_FUNC_INFO;
}

void AalMediaPlaylistControl::previous()
{
    qDebug() << Q_FUNC_INFO;
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

QT_END_NAMESPACE
