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

// This code is the C implementation of Phantom-Player: https://github.com/rsm-gh/phantom-player

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

typedef struct {
    libvlc_instance_t *instance;
    libvlc_media_player_t *player;
    libvlc_media_t *media;
} Vlc;

typedef struct {
    GtkWidget *window_root;
    GtkWidget *box_window;
    GtkWidget *box_buttons;
    GtkWidget *menubutton_restart;
    GtkWidget *menubutton_play;
    GtkWidget *menubutton_next;
    GtkWidget *scale_progress;
    GtkWidget *label_video_time;
    Vlc *vlc;

    libvlc_time_t _vlc_emitted_time;

} MediaPlayerWidget;

int digits_nb(libvlc_time_t number) {

    int digits = 1;

    if (number < 0){
        number = (number == INT64_MIN) ? INT64_MAX: -number;
    };

    while (number > 9) {
        number /= 10;
        digits++;
    }

    return digits;

};

static MediaPlayerWidget gtk_init_mp_widget(){
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
    mp_widget.vlc = NULL;

    mp_widget._vlc_emitted_time = -1;

    return mp_widget;
}

static void vlc_set_path(libvlc_instance_t *vlc_instance, libvlc_media_player_t *vlc_player, const char* path){
    libvlc_media_t *media = libvlc_media_new_path(vlc_instance, path);
    libvlc_media_player_set_media(vlc_player, media);
    // libvlc_media_release(media);
};

static Vlc vlc_init(){
    libvlc_instance_t *vlc_instance = libvlc_new(0, NULL);
    libvlc_media_player_t *vlc_player = libvlc_media_player_new(vlc_instance);
    const Vlc vlc = {vlc_instance, vlc_player, NULL};
    return vlc;
}

static void vlc_quit(libvlc_instance_t *vlc_instance, libvlc_media_player_t *vlc_player){
    libvlc_media_player_stop(vlc_player);
    libvlc_media_player_release(vlc_player);
    libvlc_release(vlc_instance);
};

static void vlc_on_time_changed(const libvlc_event_t *event, void *data)
{
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

static void gtk_on_drawing_area_realize(GtkWidget *window, gpointer user_data){

    const MediaPlayerWidget *mp_widget = user_data;

    GdkWindow* gdk_window = gtk_widget_get_window(window);
    const Window xid = gdk_x11_window_get_xid(gdk_window);

    libvlc_media_player_set_xwindow(mp_widget->vlc->player, xid);
    vlc_set_path(mp_widget->vlc->instance, mp_widget->vlc->player, "/home/rsm/Videos/vlc/test.mp4");
};

static void gtk_on_drawing_area_draw_enter(GtkWidget *widget, cairo_t *cairo_context, gpointer user_data){
    cairo_set_source_rgb(cairo_context, 0, 0, 0);
    cairo_paint(cairo_context);
};

static void gtk_on_button_play_clicked(GtkWidget *window, gpointer user_data){
    const MediaPlayerWidget *mp_widget = user_data;
    libvlc_media_player_play(mp_widget->vlc->player);
};

static void gtk_on_button_pause_clicked(GtkWidget *window, gpointer user_data){
    const MediaPlayerWidget *mp_widget = user_data;
    libvlc_media_player_pause(mp_widget->vlc->player);
};

// static void gtk_on_window_destroy(GtkWidget *window, gpointer user_data){
//     const MediaPlayerWidget *mp_widget = user_data;
//     vlc_quit(mp_widget->vlc->instance, vlc->player);
// };

static void gtk_init_mp(GtkApplication *application, MediaPlayerWidget *mp_widget){

    GtkWidget *window_root = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(window_root), "Phantom CPlayer");
    gtk_window_set_default_size(GTK_WINDOW(window_root), 800, 500);
    //g_signal_connect(window, "destroy", G_CALLBACK(gtk_on_window_destroy), user_data);
    mp_widget->window_root = window_root;

    GtkWidget *box_window = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(box_window, true);
    gtk_widget_set_vexpand(box_window, true);
    gtk_container_add(GTK_CONTAINER(window_root), box_window);
    mp_widget->box_window = box_window;

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_hexpand(drawing_area, true);
    gtk_widget_set_vexpand(drawing_area, true);
    gtk_container_add(GTK_CONTAINER(box_window), drawing_area);
    g_signal_connect(drawing_area, "realize", G_CALLBACK(gtk_on_drawing_area_realize), mp_widget);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(gtk_on_drawing_area_draw_enter), NULL);

    GtkWidget *box_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(box_window), box_buttons);
    mp_widget->box_buttons = box_buttons;

    GtkWidget *button_play = gtk_button_new_with_label("Play");
    gtk_container_add(GTK_CONTAINER(box_buttons), button_play);
    g_signal_connect(button_play, "clicked", G_CALLBACK(gtk_on_button_play_clicked), mp_widget);

    GtkWidget *button_pause = gtk_button_new_with_label("Pause");
    gtk_container_add(GTK_CONTAINER(box_buttons), button_pause);
    g_signal_connect(button_pause, "clicked", G_CALLBACK(gtk_on_button_pause_clicked), mp_widget);

    GtkWidget *scale_progress = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 1, 1);
    gtk_widget_set_hexpand(scale_progress, true);
    gtk_widget_set_sensitive(scale_progress, false);
    gtk_scale_set_draw_value(GTK_SCALE(scale_progress), false);
    gtk_container_add(GTK_CONTAINER(box_buttons), scale_progress);
    mp_widget->scale_progress = scale_progress;

    GtkWidget *label_video_time = gtk_label_new("00:00");
    gtk_container_add(GTK_CONTAINER(box_buttons), label_video_time);
    mp_widget->label_video_time = label_video_time;

    //
    // VLC
    //
    libvlc_event_manager_t *event_manager = libvlc_media_player_event_manager(mp_widget->vlc->player);

    const int status = libvlc_event_attach(event_manager, libvlc_MediaPlayerTimeChanged, vlc_on_time_changed, mp_widget);
    if (status != 0){
        printf("Error 001: libvlc_event_attach -> libvlc_MediaPlayerTimeChanged\n");
        exit(status);
    };

    gtk_widget_show_all(window_root);
}

static void gtk_on_app_activate(GtkApplication* application, gpointer user_data){
    MediaPlayerWidget *mp_widget = user_data;
    gtk_init_mp(application, mp_widget);
};

int main(const int argc, char* argv[]){

    putenv("GDK_BACKEND=x11");

    MediaPlayerWidget mp_widget = gtk_init_mp_widget();
    Vlc vlc = vlc_init();
    mp_widget.vlc = &vlc;

    GtkApplication *application = gtk_application_new("com.senties-martinelli.PhantomCPlayer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(application, "activate", G_CALLBACK(gtk_on_app_activate), &mp_widget);

    const int gtk_status = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);

    vlc_quit(vlc.instance, vlc.player);

    return gtk_status;
}