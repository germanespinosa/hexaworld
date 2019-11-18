cmake_minimum_required(VERSION 3.13)
project(hexaworld CXX)

#find_package(Ge211 CONFIG QUIET)

#find_package(Utf8-cpp CONFIG QUIET)
find_package(Cellworld CONFIG QUIET)
find_package(GComm CONFIG QUIET)

get_target_property( UTF8-CPP_INTERFACE_INCLUDE_DIRECTORIES utf8-cpp INTERFACE_INCLUDE_DIRECTORIES )

add_executable(hexamap src/hexamap.cpp)

target_link_libraries(hexamap
        PUBLIC
        cellworld)

add_executable( show src/show.cpp src/predator.cpp src/prey.cpp)
target_link_libraries(show cellworld)
include_directories(include)