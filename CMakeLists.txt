
cmake_minimum_required(VERSION 3.30)
project(cplayer C)

set(CMAKE_C_STANDARD 17)


#find_library(VLC_LIB libvlc)

#message(VLC_LIB="${VLC_LIB}")

#include_directories(/usr/include/)
#link_directories(usr/lib/)

add_executable(cplayer main.c)