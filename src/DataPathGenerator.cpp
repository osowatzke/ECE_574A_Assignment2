#include "DataPathGenerator.h"

#include "DataManager.h"
#include "FileParser.h"
#include "ImplicitComponentGenerator.h"

using namespace Parser;
using namespace Writer;

namespace DataPathGen
{

DataPathGenerator::DataPathGenerator()
    : data_manager(DataManager())
    , file_parser(FileParser(&data_manager))
    , implicit_component_generator(ImplicitComponentGenerator(&data_manager))
    , file_writer(FileWriter(&data_manager))
{}

void DataPathGenerator::run(string in_file_name, string out_file_name)
{
    file_parser.run(in_file_name);
    implicit_component_generator.run();
    file_writer.run(out_file_name);
    data_manager.print_wires();
    data_manager.print_components();
}

} // namespace DataPathGen

int main(int argc, char** argv)
{
    DataPathGenerator data_path_gen;
    data_path_gen.run(argv[1], argv[2]);
    return 0;
}