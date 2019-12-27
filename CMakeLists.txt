cmake_minimum_required( VERSION 3.13 )
project( hexaworld CXX )

find_package( Python3 COMPONENTS Interpreter Development )
find_package( Cellworld CONFIG QUIET )
find_package( GComm CONFIG QUIET )

get_target_property( UTF8-CPP_INTERFACE_INCLUDE_DIRECTORIES utf8-cpp INTERFACE_INCLUDE_DIRECTORIES )

add_executable( hexamap src/hexamap.cpp include/utils.h src/utils.cpp include/blind.h include/full_vision.h include/partial_vision.h include/hexaworld.h)

target_link_libraries( hexamap
        PUBLIC
        cellworld )

add_library(hexaworld include/predator.h src/predator.cpp include/prey.h src/prey.cpp include/utils.h src/utils.cpp)
target_link_libraries( hexaworld cellworld )

add_executable( hexasim src/hexaworld.cpp src/predator.cpp src/prey.cpp src/utils.cpp src/blind.cpp src/full_vision.cpp src/partial_vision.cpp src/map_editor.cpp)
target_link_libraries( hexasim hexaworld )
include_directories( include )

###
### TESTS CONFIGURATION
###

find_package(CatchTests CONFIG QUIET)
