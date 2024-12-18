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

#ifndef _MEDIA_PLAYER_WIDGET_H
#define _MEDIA_PLAYER_WIDGET_H

#include <vlc/vlc.h>
#include <gtk/gtk.h>
#include "VlcWidget.h"

typedef struct {
    GtkWidget *window_root;
    GtkWidget *box_window;
    GtkWidget *box_buttons;
    GtkWidget *menubutton_restart;
    GtkWidget *menubutton_play;
    GtkWidget *menubutton_next;
    GtkWidget *scale_progress;
    GtkWidget *label_video_time;
    VlcWidget *vlc;

    libvlc_time_t _vlc_emitted_time;

} MediaPlayerWidget;

MediaPlayerWidget create_media_player(VlcWidget *vlc_widget);

void init_media_player(GtkApplication *application, MediaPlayerWidget *mp_widget);

#endif //_MEDIA_PLAYER_WIDGET_H
