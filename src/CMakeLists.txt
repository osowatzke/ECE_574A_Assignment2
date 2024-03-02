SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

set(SOURCES
    ComponentParser.cpp 
    DataPathGenerator.cpp
    DataManager.cpp
    FileParser.cpp
    ImplicitComponentGenerator.cpp
    WireParser.cpp)

set(HEADER_FILES
    ComponentParser.h
    DataPathGenerator.h
    DataTypes.h
    DataManager.h    
    FileParser.h
    WireParser.h)

add_executable(dpgen ${SOURCES} ${HEADER_FILES})