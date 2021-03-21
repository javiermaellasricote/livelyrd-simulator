cmake_minimum_required(VERSION 3.15)
project(Simulator)

set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(mongocxx REQUIRED)
find_package(ModellingLibrary REQUIRED)
find_package(MathParser REQUIRED)

include_directories(${EIGEN_INCLUDE_DIR})
include_directories(${MODELLINGLIBRARY_INCLUDE_DIR})
include_directories(${MATHPARSER_INCLUDE_DIR})

file(GLOB SOURCES
        ./*.cpp
        )

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE mongo::mongocxx_shared)
target_link_libraries(${PROJECT_NAME} PUBLIC ${MODELLINGLIBRARY_LIBRARY})
target_link_libraries(${PROJECT_NAME} PUBLIC ${MATHPARSER_LIBRARY})