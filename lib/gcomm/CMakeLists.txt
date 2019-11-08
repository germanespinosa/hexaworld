cmake_minimum_required(VERSION 3.13)
project(gcomm 
            VERSION 2019.1.0
            DESCRIPTION "Cross platform object communication framework"
            LANGUAGES CXX)

include_directories(src)
include_directories(include)

add_library ( gcomm
    include/gcomm.h
    include/gsocketconnector.h
    include/gmemconnector.h
    src/gcomm.cpp
    src/gsocketconnector.cpp
    src/gmemconnector.cpp )


set_target_properties( gcomm
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED On
        CXX_EXTENSIONS Off
        VERSION ${PROJECT_VERSION} 
        RUNTIME_OUTPUT_DIRECTORY "bin/")

if(MSVC)
    target_compile_options(gcomm PRIVATE /W4)
else(MSVC)
    target_compile_options(gcomm PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)

target_include_directories(gcomm
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${SDL2_INCLUDE_DIRS})

target_include_directories(gcomm
    PRIVATE
        include )
        
target_compile_definitions(gcomm PRIVATE
    -DGE211_RESOURCES="$<JOIN:${GE211_RESOURCE_PATH},\"$<COMMA>\">")
    
include(GNUInstallDirs)

install(TARGETS gcomm  EXPORT GCommConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT GCommConfig
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/GComm)
export(TARGETS gcomm FILE GCommConfig.cmake)