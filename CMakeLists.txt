cmake_minimum_required( VERSION 3.13 )
project( hexaworld CXX )

set(CMAKE_CXX_STANDARD 17)

find_package( Cellworld CONFIG QUIET )

include_directories( include )

add_library( hexaworld
        include/agents/predator.h
        include/reward_config.h
        include/utils.h
        include/habits/simulation.h
        include/agents/preys/habit_training_prey.h
        include/habits/option.h
        include/agents/preys/test_prey.h
        include/agents/preys/planners/action_planner.h
        include/agents/preys/planner.h
        include/planner/particle_filter.h
        include/agents/preys/planner_prey.h
        include/agents/preys/planners/action_planner.h
        include/agents/preys/planners/point_planner.h
        include/agents/prey.h
        src/utils.cpp
        src/reward_config.cpp
        src/habits/habit.cpp
        src/agents/preys/habit_training_prey.cpp
        src/agents/predator.cpp
        src/habits/option.cpp
        src/agents/preys/test_prey.cpp
        src/agents/preys/planner.cpp
        src/planner/particle_filter.cpp
        src/agents/preys/planners/action_planner.cpp
        src/agents/preys/planner_prey.cpp
        src/agents/preys/planners/point_planner.cpp
        src/agents/prey.cpp)


if (CMAKE_BUILD_TYPE MATCHES Debug)
    #cmake .. -DCMAKE_BUILD_TYPE=Debug
    add_definitions(-DDEBUG=ON)
    message("-- Using DEBUG configuration")
    target_link_libraries( hexaworld PUBLIC cellworld )
else()
    #cmake .. -DCMAKE_BUILD_TYPE=Release
    message("-- Using RELEASE configuration")
    target_link_libraries( hexaworld PUBLIC cellworld )
endif()

add_executable( viewer src/tools/habit_viewer.cpp)
target_link_libraries( viewer hexaworld )

add_executable( map_creation src/tools/map_creation.cpp )
target_link_libraries( map_creation hexaworld )

add_executable( cell_world_sim src/tools/simulation.cpp)
target_link_libraries( cell_world_sim hexaworld )

add_executable( map_editor src/tools/map_editor.cpp)
target_link_libraries( map_editor hexaworld )

add_executable( train_habit src/tools/habit_train.cpp)
target_link_libraries( train_habit hexaworld )

add_executable( from_occlussions src/tools/from_occlussion.cpp)
target_link_libraries( from_occlussions hexaworld )

add_executable( find_point_of_interest src/tools/find_point_of_interest.cpp )
target_link_libraries( find_point_of_interest hexaworld )

add_executable( create_paths src/tools/create_paths.cpp )
target_link_libraries( create_paths hexaworld )

add_executable( create_spawn_locations src/tools/create_spawn_locations.cpp )
target_link_libraries( create_spawn_locations hexaworld )

add_executable( get_world_info src/tools/get_world_info.cpp)
target_link_libraries( get_world_info hexaworld )

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

cellworld_add_custom_icon(icons/bridge.png icons/destination.png)

###
### TESTS CONFIGURATION
###

find_package(CatchTests CONFIG QUIET)

###
### SIMULATION INSTALLATION
###

include(GNUInstallDirs)

install(TARGETS cell_world_sim
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})