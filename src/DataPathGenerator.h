#ifndef GUARD_DataPathGenerator_h
#define GUARD_DataPathGenerator_h

#include "DataManager.h"
#include "FileParser.h"

using namespace Parser;

namespace DataPathGen
{
class DataPathGenerator
{
    public:
        DataPathGenerator();
        void run(string file_name);
    private:
        DataManager data_manager;
        FileParser file_parser;
};
} // namespace DataPathGen;

#endif // GUARD_DataPathGenerator_h