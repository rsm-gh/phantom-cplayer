#
#  MIT License
#
#  Copyright (c) 2025 Rafael Senties Martinelli.
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.

cmake_minimum_required(VERSION 3.30)

project(phantom-cplayer C)

set(CMAKE_C_STANDARD 17)

#
# Find GTK3
#
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

#
# Find VLC
#
#find_package(libvlc REQUIRED)
#include_directories("/usr/include/")
#link_directories("/usr/lib/")

#
# Local
#

include_directories(.)
link_directories(.)

add_executable(phantom-cplayer
        src/view/VlcWidget.c
        src/view/MediaPlayerWidget.c
        src/utils.c
        src/main.c)

target_link_libraries(phantom-cplayer
        ${GTK3_LIBRARIES}
        "/usr/lib/libvlc.so"
        "/usr/lib/libvlccore.so")