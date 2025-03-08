cmake_minimum_required(VERSION 3.25)
project(Test)

set(EXAMPLE_ID "Compute" CACHE STRING "Example identifier. Available: RT / Compute / Fractal")
set(EXAMPLE_API "D3D12" CACHE STRING "Example identifier. Available: D3D12 / VULKAN / METAL")


option(ENABLE_NGFX "Enable Nvidia NGFX debugger integration.")

set(CMAKE_CXX_STANDARD 20)

set(Files
        FrameDebuggerIntegration.cpp
        FrameDebuggerIntegration.h
        Application.cpp
        Application.h
        Utils.h
        ApplicationRT.cpp
        ApplicationRT.h
        ApplicationFractal.cpp
        ApplicationFractal.h
)


if(APPLE)
    add_executable(Test main.mm ${Files})
    target_compile_definitions(Test PRIVATE RHINO_APPLE_SURFACE=1)
else()
    add_executable(Test main.cpp ${Files})
    target_link_libraries(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/bin/x64/WinPixEventRuntime.lib)
    target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/Include/)
    target_compile_definitions(Test PRIVATE RHINO_WIN32_SURFACE=1 USE_PIX=1)
    add_custom_command(TARGET Test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/WinPixEventRuntime/bin/x64/WinPixEventRuntime.dll $<TARGET_FILE_DIR:Test>
            COMMAND_EXPAND_LISTS
    )

    target_link_libraries(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/lib/x64/NGFX_Injection.lib)
    target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/include)
    if(${ENABLE_NGFX})
        target_compile_definitions(Test PRIVATE USE_NGFX=1)
        add_custom_command(TARGET Test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NsightGraphicsSDK/lib/x64/NGFX_Injection.dll $<TARGET_FILE_DIR:Test>
                COMMAND_EXPAND_LISTS
        )
    endif ()

endif()

target_compile_definitions(Test PUBLIC EXAMPLE_ID_${EXAMPLE_ID}=1 EXAMPLE_API_${EXAMPLE_API}=1)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../RHINO RHINO)
target_link_libraries(Test PUBLIC RHINO)
# target_link_libraries(Test PUBLIC SCARs)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Math3D Math3D)
target_link_libraries(Test PUBLIC Math3D)

target_include_directories(Test PUBLIC shaders)

add_subdirectory(external/glfw)
target_link_libraries(Test PUBLIC glfw)

#add_custom_command(TARGET Test POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Test> $<TARGET_FILE_DIR:Test>
#        COMMAND_EXPAND_LISTS
#)

find_package(Python3 REQUIRED)

add_dependencies(Test SCAR)
add_custom_command(TARGET Test POST_BUILD
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/prebuild.py --compiler-path $<TARGET_FILE:SCAR> --api ${EXAMPLE_API} --example-id ${EXAMPLE_ID} --out-dir $<TARGET_FILE_DIR:Test>
        COMMAND_EXPAND_LISTS
)
