cmake_minimum_required(VERSION 3.25)
project(LumoRT)

set(CMAKE_CXX_STANDARD 20)

find_package(Python3 REQUIRED)

set(HeaderFiles
        source/pch.h
)

set(SourceFiles
        source/main.cpp
        CLI/pch.h
        CLI/main.cpp
)


if(APPLE)
    add_executable(LumoRT main.mm ${Files})
    target_compile_definitions(LumoRT PRIVATE RHINO_APPLE_SURFACE=1)
else()
    add_library(LumoRT ${SourceFiles} ${HeaderFiles})
    target_precompile_headers(LumoRT PRIVATE source/pch.h)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/RHINO RHINO)
target_link_libraries(LumoRT PUBLIC RHINO)
# target_link_libraries(LumoRT PUBLIC SCARs)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Math3D Math3D)
target_link_libraries(LumoRT PUBLIC Math3D)

#add_custom_command(TARGET LumoRT POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:LumoRT> $<TARGET_FILE_DIR:LumoRT>
#        COMMAND_EXPAND_LISTS
#)

