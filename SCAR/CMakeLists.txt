cmake_minimum_required(VERSION 3.25)
project(RHINO)

set(CMAKE_CXX_STANDARD 20)

set(InterfaceHeaderFiles

        )

set(HeaderFiles
        )

set(SourceFiles
        libMain.cpp
        )

if(APPLE)

else()
    add_library(SCAR STATIC ${InterfaceHeaderFiles} ${HeaderFiles} ${SourceFiles})
endif()