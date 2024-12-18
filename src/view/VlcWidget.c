//
//  MIT License
//
//  Copyright (c) 2025 Rafael Senties Martinelli.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.


#include <vlc/vlc.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include "VlcWidget.h"

static void vlc_set_path(libvlc_instance_t *vlc_instance, libvlc_media_player_t *vlc_player, const char* path){
    libvlc_media_t *media = libvlc_media_new_path(vlc_instance, path);
    libvlc_media_player_set_media(vlc_player, media);
    // libvlc_media_release(media);
};

void vlc_widget_release(VlcWidget *vlc_widget){
    libvlc_media_player_stop(vlc_widget->player);
    libvlc_media_player_release(vlc_widget->player);
    libvlc_media_release(vlc_widget->media);
    libvlc_release(vlc_widget->instance);

    vlc_widget->drawing_area = NULL; // GTK should be killed before calling this method
    vlc_widget->instance = NULL;
    vlc_widget->player = NULL;
    vlc_widget->media = NULL;

};

static void on_drawing_area_realize(GtkWidget *window, gpointer user_data){

    const VlcWidget *vlc_widget = user_data;

    GdkWindow* gdk_window = gtk_widget_get_window(window);
    const Window xid = gdk_x11_window_get_xid(gdk_window);

    libvlc_media_player_set_xwindow(vlc_widget->player, xid);
    vlc_set_path(vlc_widget->instance, vlc_widget->player, "/home/rsm/Videos/vlc/test.mp4");
};


static void on_drawing_area_draw(GtkWidget *widget, cairo_t *cairo_context, gpointer user_data){
    cairo_set_source_rgb(cairo_context, 0, 0, 0);
    cairo_paint(cairo_context);
};

VlcWidget vlc_widget_create(){

    VlcWidget vlc_widget;

    vlc_widget.instance = libvlc_new(0, NULL);
    vlc_widget.player = libvlc_media_player_new(vlc_widget.instance);
    vlc_widget.media = NULL;

    vlc_widget.drawing_area = NULL;

    return vlc_widget;

};

void vlc_widget_bind(VlcWidget *vlc_widget){

    if (vlc_widget->drawing_area == NULL){
        printf("Error: VlcWidget does not have a drawing_area\n");
        exit(1);
    };

    g_signal_connect(vlc_widget->drawing_area, "realize", G_CALLBACK(on_drawing_area_realize), vlc_widget);
    g_signal_connect(vlc_widget->drawing_area, "draw", G_CALLBACK(on_drawing_area_draw), NULL);
};

