//
//  GPL V3
//
//  Copyright (C) 2024 Rafael Senties Martinelli.
//
//  GtkVlc.c is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License 3 as published by
//  the Free Software Foundation.
//
//  GtkVlc.c is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along this script.  If not, see <http:www.gnu.org/licenses/>.

#include <vlc/vlc.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include "GtkVlc.h"

static void on_drawing_area_realize(GtkWidget *window, gpointer user_data){

    const GtkVlc *vlc_widget = user_data;

    GdkWindow* gdk_window = gtk_widget_get_window(window);
    const Window xid = gdk_x11_window_get_xid(gdk_window);
    libvlc_media_player_set_xwindow(vlc_widget->player, xid);

};


static void on_drawing_area_draw(GtkWidget *widget, cairo_t *cairo_context, gpointer user_data){
    cairo_set_source_rgb(cairo_context, 0, 0, 0);
    cairo_paint(cairo_context);
};


GtkVlc *gtk_vlc_new(){

    GtkVlc *vlc_widget = malloc(sizeof(GtkVlc));

    vlc_widget->instance = libvlc_new(0, NULL);
    vlc_widget->player = libvlc_media_player_new(vlc_widget->instance);
    vlc_widget->media = NULL;

    vlc_widget->drawing_area = gtk_drawing_area_new();
    g_signal_connect(vlc_widget->drawing_area, "realize", G_CALLBACK(on_drawing_area_realize), vlc_widget);
    g_signal_connect(vlc_widget->drawing_area, "draw", G_CALLBACK(on_drawing_area_draw), NULL);

    return vlc_widget;

};


void gtk_vlc_release(GtkVlc *vlc_widget){
    libvlc_media_player_stop(vlc_widget->player);
    libvlc_media_player_release(vlc_widget->player);
    libvlc_media_release(vlc_widget->media);
    libvlc_release(vlc_widget->instance);

    vlc_widget->drawing_area = NULL; // GTK should be killed before calling this method
    vlc_widget->instance = NULL;
    vlc_widget->player = NULL;
    vlc_widget->media = NULL;

    free(vlc_widget);
};
