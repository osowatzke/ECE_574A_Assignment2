set(SOURCES
    ComponentParser.cpp 
    DataPathGenerator.cpp
    DataManager.cpp
    WireParser.cpp
    ComponentParser.cpp 
    FileParser.cpp
    FileWriter.cpp
    CriticalPathCalculator.cpp
    ImplicitComponentGenerator.cpp)

set(HEADER_FILES
    ComponentParser.h
    DataPathGenerator.h
    DataTypes.h
    DataManager.h
    WireParser.h
    ComponentParser.h
    FileParser.h
    FileWriter.h
    CriticalPathCalculator.h
    ImplicitComponentGenerator.h)

add_executable(dpgen ${SOURCES} ${HEADER_FILES})