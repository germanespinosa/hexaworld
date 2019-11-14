cmake_minimum_required(VERSION 3.13)
project(hexaworld CXX)

add_subdirectory(lib/cellworld/)
add_subdirectory(lib/gcomm/)

find_package(Cellworld CONFIG QUIET)

include_directories(include)

add_executable( hexamap src/hexamap.cpp)
target_link_libraries(hexamap cellworld)

add_executable( show src/show.cpp src/predator.cpp lib/cellworld/src/prey.cpp)
target_link_libraries(show cellworld)

#[[

add_executable( hexaworld 
    src/hexaworld.cpp 
    src/model.cpp 
    src/view.cpp 
    src/controller.cpp )
target_link_libraries(hexaworld cellworld)
target_link_libraries(hexaworld gcomm)

set_target_properties(hexaworld PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED On
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY "bin/")

add_executable(prey src/prey.cpp)

target_link_libraries(prey cellworld)
target_link_libraries(prey gcomm)

set_target_properties(prey PROPERTIES
        CXX_STANDARD            14
        CXX_STANDARD_REQUIRED   On
        CXX_EXTENSIONS          OFF
        RUNTIME_OUTPUT_DIRECTORY "bin/")

if(MSVC)
    target_compile_options(hexaworld PRIVATE /W4)
else(MSVC)
    target_compile_options(hexaworld PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)

include(GNUInstallDirs)
install(TARGETS hexaworld
        RUNTIME DESTINATION bin)
]]