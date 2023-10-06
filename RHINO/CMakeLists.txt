cmake_minimum_required(VERSION 3.26)
project(LumoRT)

set(CMAKE_CXX_STANDARD 20)

set(InterfaceHeaderFiles
        include/RHINO.h
        include/RHINOTypes.h)

set(HeaderFiles
        source/pch.h

        source/RHINOTypesImpl.h
        source/Utils/Common.h
)

set(SourceFiles
        source/D3D12/D3D12Backend.ixx
        source/D3D12/D3D12BackendTypes.ixx
        source/RHINOTypesImpl.ixx
        source/main.cpp
)

add_library(RHINO ${SourceFiles} ${HeaderFiles} ${InterfaceHeaderFiles})
target_precompile_headers(RHINO PRIVATE source/pch.h)
target_include_directories(RHINO PUBLIC include)
target_include_directories(RHINO PUBLIC source)


target_link_libraries(RHINO PRIVATE d3d12.lib dxgi.lib)
