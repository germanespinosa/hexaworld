cmake_minimum_required( VERSION 3.13 )
project( hexaworld CXX )

find_package( Python3 COMPONENTS Interpreter Development )
find_package( Cellworld CONFIG QUIET )
find_package( GComm CONFIG QUIET )

get_target_property( UTF8-CPP_INTERFACE_INCLUDE_DIRECTORIES utf8-cpp INTERFACE_INCLUDE_DIRECTORIES )

include_directories( include )

add_library( hexaworld
        include/predator.h
        include/hexaworld.h
        include/utils.h
        include/habit.h
        include/habit_training.h
        include/policy_training.h
        include/action_set.h
        src/utils.cpp
        src/hexaworld.cpp
        src/habit.cpp
        src/agents/habit_training.cpp
        src/agents/predator.cpp
        src/action_set.cpp )

target_link_libraries( hexaworld PUBLIC cellworld )

add_executable( map_creation src/tools/map_creation.cpp)
target_link_libraries( map_creation hexaworld )

add_executable( simulation src/tools/simulation.cpp)
target_link_libraries( simulation hexaworld )

add_executable( map_editor src/tools/map_editor.cpp)
target_link_libraries( map_editor hexaworld )

add_executable( train src/tools/habit_train.cpp)
target_link_libraries( train hexaworld )

add_executable( square src/tools/square_world.cpp)
target_link_libraries( square hexaworld )

###
### TESTS CONFIGURATION
###

configure_file(python/map.py ${CMAKE_CURRENT_BINARY_DIR}/map.py COPYONLY)

find_package(CatchTests CONFIG QUIET)
