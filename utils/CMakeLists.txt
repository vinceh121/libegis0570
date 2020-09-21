cmake_minimum_required (VERSION 2.6)

include(GNUInstallDirs)

#  VERSION "0.0.1"
project(libegis0570 DESCRIPTION "Utils for libegis0570")

add_executable(read egis_read.cpp) # TODO make list with foreach
target_link_libraries(read egis0570)
target_include_directories(read PUBLIC ../src)

add_executable(image egis_image.cpp)
target_link_libraries(image egis0570)
target_link_libraries(image png)
target_include_directories(image PUBLIC ../src)

add_executable(reg egis_reg.cpp)
target_link_libraries(reg egis0570)
target_include_directories(reg PUBLIC ../src)

add_executable(term egis_term.cpp)
target_link_libraries(term egis0570)
target_include_directories(term PUBLIC ../src)
