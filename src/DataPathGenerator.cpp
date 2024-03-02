#include "DataPathGenerator.h"

#include "DataManager.h"
#include "FileParser.h"
#include "ImplicitComponentGenerator.h"

#include <iostream>

using namespace DataPathGen;
using namespace Parser;
using namespace std;

namespace Parser
{

DataPathGenerator::DataPathGenerator()
    : data_manager(DataManager())
    , file_parser(FileParser(&data_manager))
    , implicit_component_generator(ImplicitComponentGenerator(&data_manager))
{}

void DataPathGenerator::run(string file_name)
{
    file_parser.run(file_name);
    cout << "Finished File Parser" << endl;
    implicit_component_generator.run();
    data_manager.print_wires();
    data_manager.print_components();
}

} // namespace DataPathGen

int main(int argc, char** argv)
{
    DataPathGenerator data_path_gen;
    data_path_gen.run(argv[1]);
    return 0;
}