
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

struct VlcContainer{
    libvlc_instance_t *instance;
    libvlc_media_player_t *player;
    libvlc_media_t *media;
};

void vlc_set_path(libvlc_instance_t *vlc_instance, libvlc_media_player_t *vlc_player, const char* path){
    libvlc_media_t *media = libvlc_media_new_path(vlc_instance, path);
    libvlc_media_player_set_media(vlc_player, media);
    libvlc_media_release(media);
};

struct VlcContainer vlc_init(){
    libvlc_instance_t *vlc_instance = libvlc_new(0, NULL);
    libvlc_media_player_t *vlc_player = libvlc_media_player_new(vlc_instance);
    const struct VlcContainer vlc_container = {vlc_instance, vlc_player, NULL};
    return vlc_container;
}

void vlc_quit(libvlc_instance_t *vlc_instance, libvlc_media_player_t *vlc_player){
    libvlc_media_player_stop(vlc_player);
    libvlc_media_player_release(vlc_player);
    libvlc_release(vlc_instance);
};

static void gtk_on_window_realize(GtkWidget *window, gpointer user_data){
    GdkWindow* gdk_window = gtk_widget_get_window(window);
    const Window xid = gdk_x11_window_get_xid(gdk_window);

    const struct VlcContainer *vlc_container = (struct VlcContainer*) user_data;
    libvlc_media_player_set_xwindow(vlc_container->player, xid);

    vlc_set_path(vlc_container->instance, vlc_container->player, "/home/rsm/Videos/vlc/test.mp4");
    libvlc_media_player_play(vlc_container->player);
};

// static void gtk_on_window_destroy(GtkWidget *window, gpointer user_data){
//     const struct VlcContainer *vlc_container =(struct VlcContainer*) user_data;
//     vlc_quit(vlc_container->instance, vlc_container->player);
// };

static void gtk_app_activate(GtkApplication* app, gpointer user_data){
    GtkWidget *window = gtk_application_window_new(app);

    g_signal_connect(window, "realize", G_CALLBACK(gtk_on_window_realize), user_data);
    //g_signal_connect(window, "destroy", G_CALLBACK(gtk_on_window_destroy), user_data);

    gtk_window_set_title(GTK_WINDOW(window), "VLC Player on GTK3");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_widget_show_all(window);
};

int main(int argc, char* argv[]){

    putenv("GDK_BACKEND=x11");

    struct VlcContainer vlc_container = vlc_init();

    GtkApplication *application = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(application, "activate", G_CALLBACK(gtk_app_activate), &vlc_container);

    const int gtk_status = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);

    vlc_quit(vlc_container.instance, vlc_container.player);

    return gtk_status;
}