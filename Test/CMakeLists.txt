cmake_minimum_required(VERSION 3.25)
project(Test)

add_executable(Test main.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../RHINO RHINO)
target_link_libraries(Test PUBLIC RHINO)