//
//  MIT License
//
//  Copyright (c) 2024 Rafael Senties Martinelli.
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
#include "GtkPlayer.h"
#include "GtkVlc.h"

#define EMPTY_VIDEO_LENGTH "00:00"
#define TEXT_BTN_PLAY "Play"
#define TEXT_PHANTOM_PLAYER "Phantom Player"

static void on_player_time_changed(const libvlc_event_t *event, void *data){

    libvlc_time_t player_time = event->u.media_player_time_changed.new_time;

    // Convert to seconds
    if(player_time > 0){
        player_time = player_time / 1000;
    }else{
        player_time = 0;
    };

    // Update the emitted time
    GtkPlayer *player_widget = data;
    if (player_time == player_widget->_vlc_emitted_time){
        return;
    };

    player_widget->_vlc_emitted_time = player_time;

    // Update the time label
    char time_text[digits_nb(player_time)];
    sprintf(time_text, "%ld", player_time);
    gtk_label_set_text(GTK_LABEL(player_widget->label_video_time), time_text);

};

static void on_player_parse_changed(const libvlc_event_t *event, void *data){
    //const GtkPlayer *player_widget = data;

    printf("Media parsed changed event triggered.\n");

    switch(event->u.media_parsed_changed.new_status){

        case libvlc_media_parsed_status_done:
            printf("    Done.\n");
            break;

        default:
            printf("    Other status.\n");
            break;
    };
};

static void on_button_play_clicked(GtkWidget *window, gpointer user_data){
    const GtkPlayer *player_widget = user_data;
    libvlc_media_player_play(player_widget->vlc->player);
};

// static void on_button_pause_clicked(GtkWidget *window, gpointer user_data){
//     const GtkPlayer *player_widget = user_data;
//     libvlc_media_player_pause(player_widget->vlc->player);
// };

static void gtk_plyer_free_media(GtkPlayer *player_widget){

    if (player_widget->vlc->media != NULL){
        libvlc_media_release(player_widget->vlc->media);
        player_widget->vlc->media = NULL;
    };

    if (player_widget->vlc->media_event_manager != NULL){
        libvlc_event_detach(player_widget->vlc->media_event_manager, libvlc_MediaParsedChanged, on_player_parse_changed, player_widget);
        player_widget->vlc->media_event_manager = NULL;
    };

};

void gtk_player_set_path(GtkPlayer *player_widget, const char* path){

    gtk_plyer_free_media(player_widget);

    libvlc_media_t *media = libvlc_media_new_path(player_widget->vlc->instance, path);

    if (!media) {
        fprintf(stderr, "Failed to create media object.\n");
        return;
    };

    player_widget->vlc->media_event_manager = libvlc_media_event_manager(media);

    if (libvlc_event_attach(player_widget->vlc->media_event_manager, libvlc_MediaParsedChanged, on_player_parse_changed, player_widget) != 0){
        printf("Error 002: libvlc_event_attach -> libvlc_MediaParsedChanged\n");
        exit(1);
    };


    libvlc_media_player_set_media(player_widget->vlc->player, media);
    player_widget->vlc->media = media;
    // libvlc_media_release(media);

    libvlc_media_parse_with_options(media, libvlc_media_parse_local, -1);
    printf("parsing launched\n");
};

static void on_window_destroy(GtkWidget *window, gpointer user_data){
    GtkPlayer *player_widget = user_data;
    libvlc_event_detach(player_widget->vlc->player_event_manager, libvlc_MediaParsedChanged, on_player_parse_changed, user_data);
    gtk_plyer_free_media(player_widget);
    gtk_vlc_release(player_widget->vlc);
    free(player_widget);
};

GtkPlayer *gtk_player_new(GtkApplication *application){

    GtkPlayer *player_widget = malloc(sizeof(GtkPlayer));

    player_widget->window_root = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(player_widget->window_root), TEXT_PHANTOM_PLAYER);
    gtk_window_set_default_size(GTK_WINDOW(player_widget->window_root), 800, 500);
    g_signal_connect(player_widget->window_root, "destroy", G_CALLBACK(on_window_destroy), player_widget);

    player_widget->box_window = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(player_widget->box_window, true);
    gtk_widget_set_vexpand(player_widget->box_window, true);
    gtk_container_add(GTK_CONTAINER(player_widget->window_root), player_widget->box_window);

    player_widget->vlc = gtk_vlc_new();
    gtk_widget_set_hexpand(player_widget->vlc->drawing_area, true);
    gtk_widget_set_vexpand(player_widget->vlc->drawing_area, true);
    gtk_container_add(GTK_CONTAINER(player_widget->box_window), player_widget->vlc->drawing_area);

    player_widget->box_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(player_widget->box_window), player_widget->box_buttons);

    player_widget->menubutton_play = gtk_button_new_with_label(TEXT_BTN_PLAY);
    gtk_container_add(GTK_CONTAINER(player_widget->box_buttons), player_widget->menubutton_play);
    g_signal_connect(player_widget->menubutton_play, "clicked", G_CALLBACK(on_button_play_clicked), player_widget);

    // GtkWidget *button_pause = gtk_button_new_with_label("Pause");
    // gtk_container_add(GTK_CONTAINER(player_widget->box_buttons), button_pause);
    // g_signal_connect(button_pause, "clicked", G_CALLBACK(on_button_pause_clicked), player_widget);

    player_widget->scale_progress = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 1, 1);
    gtk_widget_set_hexpand(player_widget->scale_progress, true);
    gtk_widget_set_sensitive(player_widget->scale_progress, false);
    gtk_scale_set_draw_value(GTK_SCALE(player_widget->scale_progress), false);
    gtk_container_add(GTK_CONTAINER(player_widget->box_buttons), player_widget->scale_progress);

    player_widget->label_video_time = gtk_label_new(EMPTY_VIDEO_LENGTH);
    gtk_container_add(GTK_CONTAINER(player_widget->box_buttons), player_widget->label_video_time );

    //
    // Extra
    //
    player_widget->_vlc_emitted_time = -1;

    //
    // VLC
    //
    player_widget->vlc->player_event_manager = libvlc_media_player_event_manager(player_widget->vlc->player);

    if (libvlc_event_attach(player_widget->vlc->player_event_manager, libvlc_MediaPlayerTimeChanged, on_player_time_changed, player_widget) != 0){
        printf("Error 001: libvlc_event_attach -> libvlc_MediaPlayerTimeChanged\n");
        exit(1);
    };

    return player_widget;

};
