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

#include "view/GtkPlayer.h"

static void on_activate(GtkApplication *application, gpointer user_data){
    const GtkPlayer *player_widget = gtk_player_new(application);
    gtk_widget_show_all(player_widget->window_root);
};

static void on_open(GApplication *application, GFile **files, gint n_files, const gchar *hint){

    const GtkPlayer *player_widget = gtk_player_new(GTK_APPLICATION(application));
    gtk_widget_show_all(player_widget->window_root);

    for (gint i = 0; i < n_files; i++) {
        char *video_path = g_file_get_path(files[i]);

        if (i == 0){
            gtk_player_set_path(player_widget, video_path);
        }else{
            g_print("File will not be open: %s\n", video_path);
        };

        g_free(video_path);
    };
}

int main(const int argc, char *argv[]){

    putenv("GDK_BACKEND=x11");

    GtkApplication *application = gtk_application_new("com.senties-martinelli.PhantomCPlayer", G_APPLICATION_HANDLES_OPEN);

    g_signal_connect(application, "activate", G_CALLBACK(on_activate), NULL);
    g_signal_connect(application, "open", G_CALLBACK(on_open), NULL);

    const int gtk_status = g_application_run(G_APPLICATION(application), argc, argv);

    g_object_unref(application);

    return gtk_status;
}
