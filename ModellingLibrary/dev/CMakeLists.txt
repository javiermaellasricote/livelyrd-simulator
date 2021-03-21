cmake_minimum_required(VERSION 3.15)
project(ModellingLibrary)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

include_directories(./thirdPartyLibraries/eigen)

file(GLOB SOURCES
        ./*.h
        ./*.cpp
        UnitTesting/*.cpp
        UnitTesting/*.h
)

add_executable(ModellingLibraryTesting main.cpp ${SOURCES})
target_link_libraries(ModellingLibraryTesting PRIVATE ModellingLibrary)