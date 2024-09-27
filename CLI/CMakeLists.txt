cmake_minimum_required(VERSION 3.25)
project(LumoRTCLI)

set(CMAKE_CXX_STANDARD 20)

find_package(Python3 REQUIRED)
find_package(Git REQUIRED)

set(HeaderFiles
        pch.h
)

set(SourceFiles
        main.cpp
)


if(APPLE)
    add_executable(LumoRTCLI main.mm ${Files})
    target_compile_definitions(LumoRTCLI PRIVATE RHINO_APPLE_SURFACE=1)
else()
    if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/vcpkg)
        execute_process(COMMAND ${GIT_EXECUTABLE} clone git@github.com:microsoft/vcpkg.git --single-branch
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        ECHO_ERROR_VARIABLE
                        COMMAND_ERROR_IS_FATAL LAST)
        execute_process(COMMAND bootstrap-vcpkg.bat
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/vcpkg
                        ECHO_ERROR_VARIABLE
                        COMMAND_ERROR_IS_FATAL LAST)
        execute_process(COMMAND vcpkg.exe install usd:x64-windows
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/vcpkg
                        ECHO_ERROR_VARIABLE
                        COMMAND_ERROR_IS_FATAL LAST)
    endif()
    include(${CMAKE_CURRENT_BINARY_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
    find_package(OpenVDB CONFIG GLOBAL REQUIRED)

    add_library(LumoRTCLI ${SourceFiles} ${HeaderFiles})
    target_precompile_headers(LumoRTCLI PRIVATE source/pch.h)
endif()

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH LUMO_RT_ROOT_DIR)
add_subdirectory(${LUMO_RT_ROOT_DIR} LumoRT)
target_link_libraries(LumoRTCLI PUBLIC LumoRT)

#add_custom_command(TARGET LumoRTCLI POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:LumoRTCLI> $<TARGET_FILE_DIR:LumoRTCLI>
#        COMMAND_EXPAND_LISTS
#)

