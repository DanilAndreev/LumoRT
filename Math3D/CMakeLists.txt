cmake_minimum_required(VERSION 3.25)
project(Math3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SourceList
        source/Math3DMatrixFuntions.cpp
        source/Math3DEssentials.cpp
        source/Math3DVectorFunctions.cpp
        )

set(HeaderList
        include/Math3DTypes.h
        include/Math3DMatrixFunctions.h
        include/Math3DEssentials.h
        include/Math3DVectorFunctions.h
        include/Math3D.h
        include/Math3DHelpers.h)

add_library(Math3D ${SourceList} ${HeaderList})
set_target_properties(Math3D PROPERTIES LINKER_LANGUAGE CXX)
set_source_files_properties(${HeaderList} PROPERTIES HEADER_FILE_ONLY TRUE)

target_include_directories(Math3D PUBLIC include)
