#ifndef GUARD_DataPathGenerator_h
#define GUARD_DataPathGenerator_h

#include "DataManager.h"
#include "FileParser.h"
#include "ImplicitComponentGenerator.h"

using namespace Parser;

using namespace DataPathGen;
namespace Parser
{
class DataPathGenerator
{
    public:
        DataPathGenerator();
        void run(string file_name);
    private:
        DataManager data_manager;
        FileParser file_parser;
        ImplicitComponentGenerator implicit_component_generator;
};
} // namespace DataPathGen;

#endif // GUARD_DataPathGenerator_h