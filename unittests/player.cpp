/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Jim Hodapp <jim.hodapp@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "media_compatibility_layer.h"

#include <QtGlobal>

// Mock object so that we don't get an incomplete type compiler error
struct MediaPlayerWrapper
{
    MediaPlayerWrapper() { }
};

void android_media_set_video_size_cb(MediaPlayerWrapper *mp, on_msg_set_video_size cb, void *context)
{
    Q_UNUSED(mp);
    Q_UNUSED(cb);
    Q_UNUSED(context);
}

void android_media_set_video_texture_needs_update_cb(MediaPlayerWrapper *mp, on_video_texture_needs_update cb, void *context)
{
    Q_UNUSED(mp);
    Q_UNUSED(cb);
    Q_UNUSED(context);
}

void android_media_set_error_cb(MediaPlayerWrapper *mp, on_msg_error cb, void *context)
{
    Q_UNUSED(mp);
    Q_UNUSED(cb);
    Q_UNUSED(context);
}

void android_media_set_playback_complete_cb(MediaPlayerWrapper *mp, on_playback_complete cb, void *context)
{
    Q_UNUSED(mp);
    Q_UNUSED(cb);
    Q_UNUSED(context);
}

void android_media_set_media_prepared_cb(MediaPlayerWrapper *mp, on_media_prepared cb, void *context)
{
    Q_UNUSED(mp);
    Q_UNUSED(cb);
    Q_UNUSED(context);
}

MediaPlayerWrapper *android_media_new_player()
{
    return new MediaPlayerWrapper();
}

int android_media_set_data_source(MediaPlayerWrapper *mp, const char* url)
{
    Q_UNUSED(mp);
    Q_UNUSED(url);
    return 0;
}

int android_media_set_preview_texture(MediaPlayerWrapper *mp, int texture_id)
{
    Q_UNUSED(mp);
    Q_UNUSED(texture_id);
    return 0;
}

void android_media_update_surface_texture(MediaPlayerWrapper *mp)
{
    Q_UNUSED(mp);
}

void android_media_surface_texture_get_transformation_matrix(MediaPlayerWrapper *mp, GLfloat* matrix)
{
    Q_UNUSED(mp);
    Q_UNUSED(matrix);
}

int android_media_play(MediaPlayerWrapper *mp)
{
    Q_UNUSED(mp);
    return 0;
}

int android_media_pause(MediaPlayerWrapper *mp)
{
    Q_UNUSED(mp);
    return 0;
}

int android_media_stop(MediaPlayerWrapper *mp)
{
    Q_UNUSED(mp);
    return 0;
}

bool android_media_is_playing(MediaPlayerWrapper *mp)
{
    Q_UNUSED(mp);
    return true;
}

int android_media_seek_to(MediaPlayerWrapper *mp, int msec)
{
    Q_UNUSED(mp);
    Q_UNUSED(msec);
    return 0;
}

int android_media_get_current_position(MediaPlayerWrapper *mp, int *msec)
{
    Q_UNUSED(mp);
    Q_UNUSED(msec);
    return 0;
}

int android_media_get_duration(MediaPlayerWrapper *mp, int *msec)
{
    Q_UNUSED(mp);
    Q_UNUSED(msec);
    return 0;
}

int android_media_get_volume(MediaPlayerWrapper *mp, int *volume)
{
    Q_UNUSED(mp);
    Q_UNUSED(volume);
    return 0;
}

int android_media_set_volume(MediaPlayerWrapper *mp, int volume)
{
    Q_UNUSED(mp);
    Q_UNUSED(volume);
    return 0;
}
