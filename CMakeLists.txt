cmake_minimum_required(VERSION 3.13)
project(hexaworld CXX)

find_package(Cellworld CONFIG QUIET)
find_package(GComm CONFIG QUIET)

get_target_property( UTF8-CPP_INTERFACE_INCLUDE_DIRECTORIES utf8-cpp INTERFACE_INCLUDE_DIRECTORIES )

add_executable( hexamap src/hexamap.cpp include/utils.h)

target_link_libraries( hexamap
        PUBLIC
        cellworld )

add_executable( run src/hexaworld.cpp src/predator.cpp src/prey.cpp src/utils.cpp)
target_link_libraries( run cellworld )
include_directories( include )