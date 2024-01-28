cmake_minimum_required(VERSION 3.25)
project(Test)

add_executable(Test
        main.cpp
        FrameDebuggerIntegration.cpp
        FrameDebuggerIntegration.h)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../RHINO RHINO)
target_link_libraries(Test PUBLIC RHINO)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../SCAR SCAR)
target_link_libraries(Test PUBLIC SCAR)

if(APPLE)
else()
    target_link_libraries(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/bin/x64/WinPixEventRuntime.lib)
    target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/Include/)
    target_compile_definitions(Test PRIVATE USE_PIX=1)

    target_link_libraries(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/lib/x64/NGFX_Injection.lib)
    target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/include)
endif()
