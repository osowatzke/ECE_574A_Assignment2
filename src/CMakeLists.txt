set(SOURCES
    ParserDataManager.cpp
    WireParser.cpp
    ComponentParser.cpp 
    FileParser.cpp)

set(HEADER_FILES
    ParserTypes.h
    ParserDataManager.h
    WireParser.h
    ComponentParser.h
    FileParser.h)

add_executable(FileParser ${SOURCES} ${HEADER_FILES})