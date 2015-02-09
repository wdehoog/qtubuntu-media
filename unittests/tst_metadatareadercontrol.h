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

#ifndef TST_METADATAREADERCONTROL_H
#define TST_METADATAREADERCONTROL_H

#include <core/media/player.h>

#include <memory>

#include <QObject>

class AalMediaPlayerService;
class AalMediaPlayerControl;
class QMediaControl;
class QMediaPlayer;

class tst_MetaDataReaderControl : public QObject
{
    Q_OBJECT

    AalMediaPlayerService *m_service;
    AalMediaPlayerControl *m_mediaPlayerControl;
    QMediaControl *m_playerControl;
    std::shared_ptr<core::ubuntu::media::Player> m_hubPlayer;
    std::shared_ptr<core::ubuntu::media::Service> m_hubService;
    QMediaPlayer *m_qMediaPlayer;

private Q_SLOTS:
    // We want the setup to be run prior to every test case to
    // ensure correct test isolation, see http://qt-project.org/doc/qt-5/qtest-overview.html.
    void initTestCase();

    void getResolution();
};

#endif // TST_METADATAREADERCONTROL_H
