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

#include <QtCore/QtCore>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "logging.h"

class QUbuntuMedia : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QUrl> playlist READ playlist)
    Q_PROPERTY(QObject* mediaPlayer READ mediaPlayer WRITE setMediaPlayer NOTIFY mediaPlayerChanged)
    Q_ENUMS(PlaybackMode)
        
public:
    enum PlaybackMode {
        CurrentItemOnce,
        CurrentItemInLoop,
        Sequential,
        Loop,
        Random
    };

    QUbuntuMedia(QObject* parent = 0);
    
    Q_INVOKABLE void setPlaylist(const QList<QUrl> &playlist, int index = 1, PlaybackMode mode = Sequential);
    
    QList<QUrl> playlist() const { return m_qmlPlaylist; }
    QObject* mediaPlayer() const { return m_player; }
    void setMediaPlayer(QObject *mediaPlayer);


Q_SIGNALS:
    void playlistChanged();
    void mediaPlayerChanged();

private:
    QMediaPlayer *m_player;
    QList<QUrl> m_qmlPlaylist;
    QMediaPlaylist *m_mediaPlaylist;
    QMediaContent m_content; 
    int m_currIndex;
};
