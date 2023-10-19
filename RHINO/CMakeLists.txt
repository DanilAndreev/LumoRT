cmake_minimum_required(VERSION 3.25)
project(LumoRT)

set(CMAKE_CXX_STANDARD 20)

set(InterfaceHeaderFiles
        include/RHINO.h
        include/RHINOTypes.h)

set(HeaderFiles
        source/pch.h

        source/RHINOTypesImpl.h
        source/Utils/Common.h
        source/Utils/PlatformBase.h)

set(ObjCHeaderFiles
        source/Metal/MetalBackendTypes.h)
set(ObjCSourceFiles
        source/Metal/MetalBackend.mm)

set(ModuleSourceFiles
        source/D3D12/D3D12Backend.ixx
        source/D3D12/D3D12BackendTypes.ixx
        source/Vulkan/VulkanBackend.ixx
        source/Vulkan/VulkanBackendTypes.ixx)

set(SourceFiles
        source/main.cpp
        source/RHINOTypesImpl.cpp
        source/Utils/PlatformBase.h
        source/Metal/MetalBackend.h
        source/Metal/AllocateMetalBackend.h
        ../Test/main.cpp)

if(APPLE)
    add_library(RHINO ${SourceFiles} ${HeaderFiles} ${ObjCHeaderFiles} ${ObjCSourceFiles} ${InterfaceHeaderFiles})
    set_source_files_properties(${ObjCHeaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
    target_compile_options(RHINO PUBLIC -fmodules -std=c++20)
    target_compile_definitions(RHINO PRIVATE ENABLE_API_METAL=1)
else()
    add_library(RHINO ${SourceFiles} ${HeaderFiles} ${ModuleSourceFiles} ${InterfaceHeaderFiles})
    target_compile_definitions(RHINO PRIVATE ENABLE_API_D3D12=1 ENABLE_API_VULKAN=1)
    target_link_libraries(RHINO PRIVATE d3d12.lib dxgi.lib)

    find_package(Vulkan REQUIRED)
    target_include_directories(RHINO PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(RHINO PRIVATE Vulkan::Vulkan)
endif()
# set_source_files_properties(${HeaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
target_precompile_headers(RHINO PRIVATE source/pch.h)
target_include_directories(RHINO PUBLIC include)
target_include_directories(RHINO PUBLIC source)



