cmake_minimum_required (VERSION 2.6)

include(GNUInstallDirs)

#  VERSION "0.0.1"
project(egis0570 DESCRIPTION "C++ library to communicate with EGIS0570 fingerprint reader")

add_library(egis0570 SHARED src/egis0570.cpp)

target_link_libraries(egis0570 usb-1.0)

set_target_properties(egis0570 PROPERTIES PUBLIC_HEADER src/egis0570.hpp)

add_subdirectory(utils)

install(TARGETS egis0570
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


