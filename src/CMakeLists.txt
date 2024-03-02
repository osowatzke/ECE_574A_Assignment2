SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

set(SOURCES
    DataPathGenerator.cpp
    DataManager.cpp
    WireParser.cpp
    ComponentParser.cpp 
    FileParser.cpp
    ImplicitComponentGenerator.cpp)

set(HEADER_FILES
    DataPathGenerator.h
    DataTypes.h
    DataManager.h
    WireParser.h
    ComponentParser.h
    FileParser.h
    ImplicitComponentGenerator.h)

add_executable(dpgen ${SOURCES} ${HEADER_FILES})