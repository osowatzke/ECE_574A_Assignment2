#ifndef GUARD_DataPathGenerator_h
#define GUARD_DataPathGenerator_h

#include "CriticalPathCalculator.h"
#include "DataManager.h"
#include "FileParser.h"
#include "FileWriter.h"
#include "ImplicitComponentGenerator.h"

using namespace CriticalPath;
using namespace Parser;
using namespace Writer;

namespace DataPathGen
{
class DataPathGenerator
{
    public:
        DataPathGenerator();
        int run(string in_file_name, string out_file_name);
    private:
        DataManager data_manager;
        FileParser file_parser;
        ImplicitComponentGenerator component_generator;
        FileWriter file_writer;
        CriticalPathCalculator critical_path;
};
} // namespace DataPathGen;

#endif // GUARD_DataPathGenerator_h