#include "DataPathGenerator.h"

#include "DataManager.h"
#include "FileParser.h"
#include "ImplicitComponentGenerator.h"

#include <iostream>
#include <string>

namespace DataPathGen
{

// Class constructor creates the data manager and provides subclasses
// with pointer access to the data manager 
DataPathGenerator::DataPathGenerator()
    : data_manager(DataManager())
    , file_parser(FileParser(&data_manager))
    , component_generator(ImplicitComponentGenerator(&data_manager))
    , file_writer(FileWriter(&data_manager))
    , critical_path(CriticalPathCalculator(&data_manager)) {}

// Function runs the data path generator
int DataPathGenerator::run(string in_file_name, string out_file_name)
{
    // Attempt to parse the file parser, stop run if file parser fails
    int retVal = file_parser.run(in_file_name);
    if (retVal)
    {
        return retVal;
    }

    // Run the implicit component generator
    component_generator.run();

    // Attempt to run the file writer, stop run if file writer fails
    retVal = file_writer.run(out_file_name);
    if (retVal)
    {
        return retVal;
    }

    // Run the critical path calculation
    critical_path.run();
    return 0;
}

} // namespace DataPathGen

// Main routine
int main(int argc, char** argv)
{
    // Only 1 additional argument provided
    if (argc == 2)
    {
        // Handle -h flag for function help
        if(string(argv[1]) == "-h")
        {
            cout << "Description:" << endl << endl;
            cout << "Function converts a behavioral netlist into a verilog file and computes the critical path." << endl << endl;
            cout << "Usage:" << endl << endl;
            cout << "dpgen netlistFile verilogFile" << endl << endl;
            return 0;
        }
    }

    // 2 additional arguments provided
    else if (argc == 3)
    {
        // Run data path generator
        DataPathGenerator data_path_gen; 
        return data_path_gen.run(argv[1], argv[2]);
    }

    // Invalid input arguments. Provide usage for end user
    cout << "ERROR: Invalid Arguments" << endl << endl;
    cout << "Usage:" << endl << endl;
    cout << "dpgen netlistFile verilogFile" << endl << endl;
    return 1;    
}