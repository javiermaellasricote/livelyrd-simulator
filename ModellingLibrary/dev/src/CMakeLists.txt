
include_directories(../thirdPartyLibraries/eigen)

file(GLOB LIBRARY_SOURCES
        DataInfrastructure/*.h
        DataInfrastructure/*.cpp
        Engines/*.h
        Engines/*.cpp
        )

add_library(ModellingLibrary ${LIBRARY_SOURCES})