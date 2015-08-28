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

#include <QMediaContent>
#include <QMediaResource>
#include <QObject>

class AalMetaDataReaderControl;
class AalMediaPlayerService;
class AalMediaPlayerControl;
class QMediaControl;
class QMediaPlayer;

class tst_MetaDataReaderControl : public QObject
{
    Q_OBJECT

    AalMetaDataReaderControl *m_mdControlVideo;
    AalMetaDataReaderControl *m_mdControlAudio;
    AalMetaDataReaderControl *m_mdControlImage;
    QMediaResource m_mediaResource;
    QMediaContent m_media;

Q_SIGNALS:
    void onMediaChanged(const QMediaContent &media);

private Q_SLOTS:
    // We want the setup to be run prior to every test case to
    // ensure correct test isolation, see http://qt-project.org/doc/qt-5/qtest-overview.html.
    void initTestCase();
    void cleanupTestCase();

    void verifyResolution();
    void verifyDuration();
    void verifyTitle();
    void verifyAuthor();
    void verifyGenre();

    void verifyDate();
    void verifyAlbumTitle();
    void verifyAlbumArtist();

    void verifyLatitude();
    void verifyLongitude();

private:
    void setVideoMediaResource();
    void setAudioMediaResource();
    void setImageMediaResource();
};

#endif // TST_METADATAREADERCONTROL_H
