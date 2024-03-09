#include "DataPathGenerator.h"

#include "DataManager.h"
#include "FileParser.h"
#include "ImplicitComponentGenerator.h"

#include <iostream>
#include <string>

using namespace Parser;
using namespace Writer;

namespace DataPathGen
{

DataPathGenerator::DataPathGenerator()
    : data_manager(DataManager())
    , file_parser(FileParser(&data_manager))
    , component_generator(ImplicitComponentGenerator(&data_manager))
    , file_writer(FileWriter(&data_manager))
    , critical_path(CriticalPathCalculator(&data_manager))
{}

int DataPathGenerator::run(string in_file_name, string out_file_name)
{
    int retVal = file_parser.run(in_file_name);
    if (retVal)
    {
        return retVal;
    }
    component_generator.run();
    retVal = file_writer.run(out_file_name);
    if (retVal)
    {
        return retVal;
    }
    critical_path.run();
    // data_manager.print_wires();
    // data_manager.print_components();
    return 0;
}

} // namespace DataPathGen

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        if(string(argv[1]) == "-h")
        {
            cout << "Description:" << endl << endl;
            cout << "Function converts a behavioral netlist into a verilog file and computes the critical path." << endl << endl;
            cout << "Usage:" << endl << endl;
            cout << "dpgen netlistFile verilogFile" << endl << endl;
            return 0;
        }
    }
    else if (argc == 3)
    {
        DataPathGenerator data_path_gen; 
        return data_path_gen.run(argv[1], argv[2]);
    }
    cout << "ERROR: Invalid Arguments" << endl << endl;
    cout << "Usage:" << endl << endl;
    cout << "dpgen netlistFile verilogFile" << endl << endl;
    return 1;    
}