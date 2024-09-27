cmake_minimum_required(VERSION 3.25)
project(LumoRTCLI)

set(CMAKE_CXX_STANDARD 20)

find_package(Python3 REQUIRED)

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
    add_library(LumoRTCLI ${SourceFiles} ${HeaderFiles})
    target_precompile_headers(LumoRTCLI PRIVATE source/pch.h)
endif()

message(AAAA: ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../OpenUSD/build_scripts/build_usd.py ${CMAKE_CURRENT_BINARY_DIR}/OpenUSD --no-python --no-tools --no-examples --no-tutorials --no-debug-python --build-monolithic)
execute_process(COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../OpenUSD/build_scripts/build_usd.py ${CMAKE_CURRENT_BINARY_DIR}/OpenUSD --no-python --no-tools --no-examples --no-tutorials --no-debug-python --build-monolithic
                ECHO_ERROR_VARIABLE
                COMMAND_ERROR_IS_FATAL LAST)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH LUMO_RT_ROOT_DIR)
add_subdirectory(${LUMO_RT_ROOT_DIR} LumoRT)
target_link_libraries(LumoRTCLI PUBLIC LumoRT)

#add_custom_command(TARGET LumoRTCLI POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:LumoRTCLI> $<TARGET_FILE_DIR:LumoRTCLI>
#        COMMAND_EXPAND_LISTS
#)

