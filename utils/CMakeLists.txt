cmake_minimum_required (VERSION 2.6)

include(GNUInstallDirs)

#  VERSION "0.0.1"
project(libegis0570 DESCRIPTION "Utils for libegis0570")

add_executable(read egis_read.cpp)
target_link_libraries(read egis0570)
target_include_directories(read PUBLIC ../src)
