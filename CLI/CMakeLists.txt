cmake_minimum_required(VERSION 3.25)
project(LumoRTCLI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Python3 REQUIRED)
find_package(Git REQUIRED)

set(HeaderFiles
        pch.h
)

set(SourceFiles
        main.cpp
)


if(APPLE)
    if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/OpenUSD)
        if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/OpenUSD_SRC)
            execute_process(COMMAND ${GIT_EXECUTABLE} clone git@github.com:PixarAnimationStudios/OpenUSD.git OpenUSD_SRC
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    ECHO_ERROR_VARIABLE
                    COMMAND_ERROR_IS_FATAL LAST)
        endif()
        message("Building OpenUSD... This may take a while, go make yourself a coffe ;)")
        execute_process(COMMAND ${Python3_EXECUTABLE} build_scripts/build_usd.py ${CMAKE_CURRENT_BINARY_DIR}/OpenUSD --no-python --no-tools --no-examples --no-tutorials --no-debug-python --build-monolithic
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/OpenUSD_SRC
                ECHO_ERROR_VARIABLE
                COMMAND_ERROR_IS_FATAL LAST)
        message("Finished building OpenUSD")
    endif()
    set(PXR_USD_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/OpenUSD)

    add_executable(LumoRTCLI ${SourceFiles} ${HeaderFiles})
    target_compile_definitions(LumoRTCLI PRIVATE RHINO_APPLE_SURFACE=1)
    target_precompile_headers(LumoRTCLI PRIVATE pch.h)

    find_package(USD REQUIRED)
    target_include_directories(LumoRTCLI PRIVATE ${USD_INCLUDE_DIR})
    target_link_libraries(LumoRTCLI PRIVATE ${USD_LIBRARY_DIR}/libusd_ms.dylib)
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
    target_precompile_headers(LumoRTCLI PRIVATE pch.h)
endif()

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH LUMO_RT_ROOT_DIR)
add_subdirectory(${LUMO_RT_ROOT_DIR} LumoRT)
target_link_libraries(LumoRTCLI PUBLIC LumoRT)

#add_custom_command(TARGET LumoRTCLI POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:LumoRTCLI> $<TARGET_FILE_DIR:LumoRTCLI>
#        COMMAND_EXPAND_LISTS
#)

