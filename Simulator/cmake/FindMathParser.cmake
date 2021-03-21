set(FIND_MATHPARSER_PATHS
        ../MathParser/bin)

find_path(MATHPARSER_INCLUDE_DIR MathParser.h
        PATH_SUFFIXES include
        PATHS ${FIND_MATHPARSER_PATHS})

find_library(MATHPARSER_LIBRARY
        NAMES MathParser
        PATH_SUFFIXES lib
        PATHS ${FIND_MATHPARSER_PATHS})