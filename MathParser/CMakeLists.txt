cmake_minimum_required(VERSION 3.15)
project(MathParser)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(ModellingLibrary REQUIRED)

include_directories(${EIGEN_INCLUDE_DIR})
include_directories(${MODELLINGLIBRARY_INCLUDE_DIR})
include_directories(./thirdPartyLibraries/catch2)
include_directories(src)

add_subdirectory(src)

file(GLOB SOURCES
        ./*.cpp
        UnitTesting/*.cpp
        )

add_executable(MathParserTesting main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${MODELLINGLIBRARY_LIBRARY})
target_link_libraries(MathParserTesting PRIVATE MathParser)