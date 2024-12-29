//
//  GPL V3
//
//  Copyright (C) 2024 Rafael Senties Martinelli.
//
//  GtkVlc.h is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License 3 as published by
//  the Free Software Foundation.
//
//  GtkVlc.h is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along this script.  If not, see <http:www.gnu.org/licenses/>.

#ifndef _GTK_VLC
#define _GTK_VLC

#include <vlc/vlc.h>
#include <gtk/gtk.h>

typedef struct {
    GtkWidget *drawing_area;
    libvlc_instance_t *instance;
    libvlc_media_player_t *player;
    libvlc_media_t *media;
    libvlc_event_manager_t *media_event_manager;
    libvlc_event_manager_t *player_event_manager;
} GtkVlc;

GtkVlc *gtk_vlc_new();
void gtk_vlc_release(GtkVlc *vlc_widget);

#endif //_GTK_VLC
