cmake_minimum_required(VERSION 3.25)
project(Test)

set(CMAKE_CXX_STANDARD 20)

set(Files
        FrameDebuggerIntegration.cpp
        FrameDebuggerIntegration.h
        Application.cpp
        Application.h
)

add_executable(Test main.cpp ${Files})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../RHINO RHINO)
target_link_libraries(Test PUBLIC RHINO)
target_link_libraries(Test PUBLIC SCARs)

if(APPLE)
else()
    target_link_libraries(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/bin/x64/WinPixEventRuntime.lib)
    target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/Include/)
    target_compile_definitions(Test PRIVATE USE_PIX=1)

    target_link_libraries(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/lib/x64/NGFX_Injection.lib)
    target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/include)
endif()
