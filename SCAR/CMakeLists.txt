cmake_minimum_required(VERSION 3.25)
project(RHINO)

set(CMAKE_CXX_STANDARD 20)

set(InterfaceHeaderFiles
        include/SCARVersion.h
        )

set(HeaderFiles
        source/archiver/PSOArchiver.h
        source/pch.h
        )

set(SourceFiles
        source/libMain.cpp
        source/archiver/PSOArchiver.cpp
)

if(APPLE)

else()
    add_library(SCAR STATIC ${InterfaceHeaderFiles} ${HeaderFiles} ${SourceFiles})
endif()

target_include_directories(SCAR PUBLIC include)
target_precompile_headers(SCAR PRIVATE source/pch.h)
