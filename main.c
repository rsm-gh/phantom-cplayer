
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

void vlc_set_path(libvlc_instance_t *vlc_instance, libvlc_media_player_t *vlc_player, const char* path){
    libvlc_media_t *media = libvlc_media_new_path (vlc_instance, path);
    libvlc_media_player_set_media(vlc_player, media);
    libvlc_media_release(media);
};

void vlc_quit(libvlc_instance_t *vlc_instance, libvlc_media_player_t *vlc_player){
    libvlc_media_player_stop(vlc_player);
    libvlc_media_player_release(vlc_player);
    libvlc_release(vlc_instance);
};

void vlc_initialize_on_xid(const Window xid){
    libvlc_instance_t *vlc_instance = libvlc_new (0, NULL);
    libvlc_media_player_t *vlc_player = libvlc_media_player_new(vlc_instance);
    libvlc_media_player_set_xwindow (vlc_player, xid);

    vlc_set_path(vlc_instance, vlc_player, "/home/rsm/Videos/vlc/test.mp4");

    libvlc_media_player_play(vlc_player);
    //sleep(10);
    //vlc_quit(vlc_instance, vlc_player);
};

static void gtk_window_realize(GtkWidget *window, gpointer user_data){
    GdkWindow* gdk_window = gtk_widget_get_window(window);
    const Window xid = gdk_x11_window_get_xid(gdk_window);
    vlc_initialize_on_xid(xid);
};

static void gtk_app_activate (GtkApplication* app, gpointer user_data){
    GtkWidget *window = gtk_application_window_new(app);

    g_signal_connect (window, "realize", G_CALLBACK (gtk_window_realize), NULL);

    gtk_window_set_title (GTK_WINDOW (window), "VLC Player on GTK3");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
    gtk_widget_show_all (window);
};

int main(int argc, char* argv[]){

    putenv("GDK_BACKEND=x11");

    GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (gtk_app_activate), NULL);

    const int gtk_status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);
    return gtk_status;
}