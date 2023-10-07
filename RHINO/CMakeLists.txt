cmake_minimum_required(VERSION 3.25)
project(LumoRT)

set(CMAKE_CXX_STANDARD 20)

set(InterfaceHeaderFiles
        include/RHINO.h
        include/RHINOTypes.h)

set(HeaderFiles
        source/pch.h

        source/RHINOTypesImpl.h
        source/Utils/Common.h)

set(ObjCHeaderFiles
        source/Metal/MetalBackendTypes.h)
set(ObjCSourceFiles
        source/Metal/MetalBackend.mm)

set(ModuleSourceFiles
        source/D3D12/D3D12Backend.ixx
        source/D3D12/D3D12BackendTypes.ixx)

set(SourceFiles
        source/main.cpp
        source/RHINOTypesImpl.cpp
        source/Utils/PlatfromBase.h
        source/Metal/MetalBackend.h
        source/Metal/AllocateMetalBackend.h)

if(APPLE)
    add_library(RHINO ${SourceFiles} ${HeaderFiles} ${InterfaceHeaderFiles} ${ObjCHeaderFiles} ${ObjCSourceFiles})
    set_source_files_properties(${ObjCHeaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
    target_compile_options(RHINO PUBLIC -fmodules -std=c++20)
else()
    add_library(RHINO ${SourceFiles} ${HeaderFiles} ${InterfaceHeaderFiles} ${ModuleSourceFiles})
endif()
set_source_files_properties(${HeaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
target_precompile_headers(RHINO PRIVATE source/pch.h)
target_include_directories(RHINO PUBLIC include)
target_include_directories(RHINO PUBLIC source)


target_link_libraries(RHINO PRIVATE d3d12.lib dxgi.lib)
