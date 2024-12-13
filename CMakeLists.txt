
cmake_minimum_required(VERSION 3.30)

project(cplayer C)

set(CMAKE_C_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

#find_library(VLC_LIB libvlc)
#find_library(VLC_LIB gtk3)

#message(VLC_LIB="${VLC_LIB}")

#include_directories(/usr/include/)
#link_directories(usr/lib/)

add_executable(cplayer main.c)

target_link_libraries(cplayer ${GTK3_LIBRARIES})