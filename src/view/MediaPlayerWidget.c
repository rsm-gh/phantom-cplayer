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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include "../utils.h"
#include "MediaPlayerWidget.h"
#include "VlcWidget.h"

#define EMPTY_VIDEO_LENGTH "00:00"
#define TEXT_BTN_PLAY "Play"

MediaPlayerWidget create_media_player(VlcWidget *vlc_widget){
    MediaPlayerWidget mp_widget;

    mp_widget.window_root = NULL;
    mp_widget.box_window = NULL;
    mp_widget.box_buttons = NULL;
    mp_widget.menubutton_restart = NULL;
    mp_widget.menubutton_play = NULL;
    mp_widget.menubutton_next = NULL;
    mp_widget.scale_progress = NULL;
    mp_widget.menubutton_next = NULL;
    mp_widget.label_video_time = NULL;
    mp_widget.vlc = vlc_widget;

    mp_widget._vlc_emitted_time = -1;

    return mp_widget;
}

static void on_player_time_changed(const libvlc_event_t *event, void *data){

    libvlc_time_t player_time = event->u.media_player_time_changed.new_time;

    // Convert to seconds
    if(player_time > 0){
        player_time = player_time / 1000;
    }else{
        player_time = 0;
    };

    // Update the emitted time
    MediaPlayerWidget *mp_widget = data;
    if (player_time == mp_widget->_vlc_emitted_time){
        return;
    };

    mp_widget->_vlc_emitted_time = player_time;

    // Update the time label
    char time_text[digits_nb(player_time)];
    sprintf(time_text, "%ld", player_time);
    gtk_label_set_text(GTK_LABEL(mp_widget->label_video_time), time_text);

};

static void on_button_play_clicked(GtkWidget *window, gpointer user_data){
    const MediaPlayerWidget *mp_widget = user_data;
    libvlc_media_player_play(mp_widget->vlc->player);
};

static void on_button_pause_clicked(GtkWidget *window, gpointer user_data){
    const MediaPlayerWidget *mp_widget = user_data;
    libvlc_media_player_pause(mp_widget->vlc->player);
};

// static void gtk_on_window_destroy(GtkWidget *window, gpointer user_data){
//     const MediaPlayerWidget *mp_widget = user_data;
//     vlc_quit(mp_widget->vlc->instance, vlc->player);
// };

void init_media_player(GtkApplication *application, MediaPlayerWidget *mp_widget){

    mp_widget->window_root = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(mp_widget->window_root), "Phantom CPlayer");
    gtk_window_set_default_size(GTK_WINDOW(mp_widget->window_root), 800, 500);
    //g_signal_connect(window, "destroy", G_CALLBACK(gtk_on_window_destroy), user_data);

    mp_widget->box_window = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(mp_widget->box_window, true);
    gtk_widget_set_vexpand(mp_widget->box_window, true);
    gtk_container_add(GTK_CONTAINER(mp_widget->window_root), mp_widget->box_window);

    mp_widget->vlc->drawing_area = gtk_drawing_area_new();
    gtk_widget_set_hexpand(mp_widget->vlc->drawing_area, true);
    gtk_widget_set_vexpand(mp_widget->vlc->drawing_area, true);
    gtk_container_add(GTK_CONTAINER(mp_widget->box_window), mp_widget->vlc->drawing_area);
    vlc_widget_bind(mp_widget->vlc);

    mp_widget->box_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(mp_widget->box_window), mp_widget->box_buttons);

    GtkWidget *button_play = gtk_button_new_with_label(TEXT_BTN_PLAY);
    gtk_container_add(GTK_CONTAINER(mp_widget->box_buttons), button_play);
    g_signal_connect(button_play, "clicked", G_CALLBACK(on_button_play_clicked), mp_widget);

    GtkWidget *button_pause = gtk_button_new_with_label("Pause");
    gtk_container_add(GTK_CONTAINER(mp_widget->box_buttons), button_pause);
    g_signal_connect(button_pause, "clicked", G_CALLBACK(on_button_pause_clicked), mp_widget);

    mp_widget->scale_progress = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 1, 1);
    gtk_widget_set_hexpand(mp_widget->scale_progress, true);
    gtk_widget_set_sensitive(mp_widget->scale_progress, false);
    gtk_scale_set_draw_value(GTK_SCALE(mp_widget->scale_progress), false);
    gtk_container_add(GTK_CONTAINER(mp_widget->box_buttons), mp_widget->scale_progress);

    mp_widget->label_video_time  = gtk_label_new(EMPTY_VIDEO_LENGTH);
    gtk_container_add(GTK_CONTAINER(mp_widget->box_buttons), mp_widget->label_video_time );

    //
    // VLC
    //
    libvlc_event_manager_t *event_manager = libvlc_media_player_event_manager(mp_widget->vlc->player);

    const int status = libvlc_event_attach(event_manager, libvlc_MediaPlayerTimeChanged, on_player_time_changed, mp_widget);
    if (status != 0){
        printf("Error 001: libvlc_event_attach -> libvlc_MediaPlayerTimeChanged\n");
        exit(status);
    };

    gtk_widget_show_all(mp_widget->window_root);
};
