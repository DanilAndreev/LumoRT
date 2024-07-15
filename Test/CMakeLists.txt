cmake_minimum_required(VERSION 3.25)
project(Test)

set(CMAKE_CXX_STANDARD 20)

set(Files
        FrameDebuggerIntegration.cpp
        FrameDebuggerIntegration.h
        Application.cpp
        Application.h
        Utils.h
        ApplicationRT.cpp
        ApplicationRT.h
)

add_executable(Test main.cpp ${Files})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../RHINO RHINO)
target_link_libraries(Test PUBLIC RHINO)
target_link_libraries(Test PUBLIC SCARs)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Math3D Math3D)
target_link_libraries(Test PUBLIC Math3D)

target_include_directories(Test PUBLIC shaders/rt)

if(APPLE)
else()
    target_link_libraries(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/bin/x64/WinPixEventRuntime.lib)
    target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/Include/)
    target_compile_definitions(Test PRIVATE USE_PIX=1)

    target_link_libraries(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/lib/x64/NGFX_Injection.lib)
    target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/include)
endif()

#add_custom_command(TARGET Test POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Test> $<TARGET_FILE_DIR:Test>
#        COMMAND_EXPAND_LISTS
#)

find_package(Python3 REQUIRED)

add_dependencies(Test SCAR)
add_custom_command(TARGET Test POST_BUILD
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/prebuild.py --compiler-path $<TARGET_FILE:SCAR> --api D3D12 --example-id RT --out-dir $<TARGET_FILE_DIR:Test>
        COMMAND_EXPAND_LISTS
)
