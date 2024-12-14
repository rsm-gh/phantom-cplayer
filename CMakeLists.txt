
cmake_minimum_required(VERSION 3.30)

project(cplayer C)

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

add_executable(cplayer main.c)

target_link_libraries(cplayer
        ${GTK3_LIBRARIES}
        "/usr/lib/libvlc.so"
        "/usr/lib/libvlccore.so")