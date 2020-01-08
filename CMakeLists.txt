cmake_minimum_required( VERSION 3.13 )
project( hexaworld CXX )

find_package( Python3 COMPONENTS Interpreter Development )
find_package( Cellworld CONFIG QUIET )
find_package( GComm CONFIG QUIET )

get_target_property( UTF8-CPP_INTERFACE_INCLUDE_DIRECTORIES utf8-cpp INTERFACE_INCLUDE_DIRECTORIES )

include_directories( include )

add_library( hexaworld
        include/blind.h
        include/full_vision.h
        include/partial_vision.h
        include/predator.h
        include/prey.h
        include/hexaworld.h
        include/utils.h
        src/blind.cpp
        src/full_vision.cpp
        src/partial_vision.cpp
        src/predator.cpp
        src/prey.cpp
        src/utils.cpp )
target_link_libraries( hexaworld PUBLIC cellworld )

add_executable( map_creation src/tools/map_creation.cpp )
target_link_libraries( map_creation hexaworld )

add_executable( simulation src/tools/simulation.cpp)
target_link_libraries( simulation hexaworld )

add_executable( map_editor src/tools/map_editor.cpp)
target_link_libraries( map_editor hexaworld )

add_executable( train src/tools/train.cpp)
target_link_libraries( train hexaworld )


###
### TESTS CONFIGURATION
###

configure_file(python/map.py ${CMAKE_CURRENT_BINARY_DIR}/map.py COPYONLY)

find_package(CatchTests CONFIG QUIET)
