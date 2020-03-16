cmake_minimum_required( VERSION 3.13 )
project( hexaworld CXX )

find_package( Cellworld CONFIG QUIET )

include_directories( include )

add_library( hexaworld
        include/predator.h
        include/hexaworld.h
        include/utils.h
        include/habit.h
        include/habit_training.h
        include/option.h
        include/test_prey.h
        include/fixed_time_prey.h
        include/cell_option.h
        include/planner_history.h
        src/utils.cpp
        src/hexaworld.cpp
        src/habit.cpp
        src/agents/habit_training.cpp
        src/agents/predator.cpp
        src/option.cpp
        src/agents/test_prey.cpp
        src/agents/fixed_time_prey.cpp
        src/cell_option.cpp
        src/planners/planner_history.cpp
        src/planners/fixed_time_planner.cpp )


if (CMAKE_BUILD_TYPE MATCHES Debug)
    #cmake .. -DCMAKE_BUILD_TYPE=Debug
    add_definitions(-DDEBUG=ON)
    message("-- Using DEBUG configuration")
    target_link_libraries( hexaworld PUBLIC cellworlddebug )
else()
    #cmake .. -DCMAKE_BUILD_TYPE=Release
    message("-- Using RELEASE configuration")
    target_link_libraries( hexaworld PUBLIC cellworld )
endif()

add_executable( viewer src/tools/habit_viewer.cpp)
target_link_libraries( viewer hexaworld )


add_executable( map_creation src/tools/map_creation.cpp )
target_link_libraries( map_creation hexaworld )

add_executable( simulation src/tools/simulation.cpp)
target_link_libraries( simulation hexaworld )

add_executable( map_editor src/tools/map_editor.cpp)
target_link_libraries( map_editor hexaworld )

add_executable( train_habit src/tools/habit_train.cpp)
target_link_libraries( train_habit hexaworld )

add_executable( square src/tools/square_world.cpp)
target_link_libraries( square hexaworld )

###
### COPY WORLDS
###

file(GLOB world_glob "worlds/*" )
file(COPY ${world_glob} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB scripts_glob "scripts/*" )
file(COPY ${scripts_glob} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

###
### COPY CUSTOM ICONS
###

cellworld_add_custom_icon(icons/bridge.png)

###
### TESTS CONFIGURATION
###

find_package(CatchTests CONFIG QUIET)

message("HERE ${CELLWORLD_RESOURCES}")