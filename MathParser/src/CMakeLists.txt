include_directories(${EIGEN_INCLUDE_DIR})
include_directories(${MODELLINGLIBRARY_INCLUDE_DIR})

file(GLOB LIBRARY_SOURCES
        *.cpp
        )


add_library(MathParser ${LIBRARY_SOURCES})