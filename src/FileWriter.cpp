#include "FileWriter.h"
#include "DataManager.h"

#include <iostream>
#include <regex>

namespace DataPathGen
{
// Class constructor provides access to shared data manager
FileWriter::FileWriter(DataManager* data_manager)
    : data_manager(data_manager) {}

// Function determines the module name
void FileWriter::determineModuleName(string filePath)
{
    // Strip file path and file extension from input file
    // find_last_of will return -1 for non-matches
    int startIdx = filePath.find_last_of("/\\") + 1;
    int endIdx = filePath.find_last_of(".");

    // Take all characters if there is no file extension
    if (endIdx < 0)
    {
        endIdx = filePath.length();
    }

    // Extract file name from file path
    string fileName = filePath.substr(startIdx, endIdx-startIdx);

    // Remove the 474a_ and 574a_ substrings from module name
    // Module name should not start with numbers
    if ((fileName.substr(0,5) == "474a_") || (fileName.substr(0,5) == "574a_"))
    {
        moduleName = fileName.substr(5);
    }
    else
    {
        moduleName = fileName;
    }
}

// Function declares the timescale at the beginning of the file
void FileWriter::declareTimescale()
{
    circuitFile << "`timescale 1ns/1ns" << endl << endl;
}

// Function declares the module at the beginning of the file
void FileWriter::declareModule()
{
    circuitFile << "module " << moduleName;
    circuitFile << "(";
    bool firstInput = true;

    // Loops through all the wires in the data manager
    for (wire*& currWire : data_manager->wires)
    {

        // Print input and output ports
        if ((currWire->type == WireType::INPUT) || (currWire->type == WireType::OUTPUT))
        {
            if (!firstInput)
            {
                circuitFile << ", ";
            }
            firstInput = false;
            circuitFile << currWire->name;
        }
    }
    circuitFile << ");" << endl << endl;
}

// Function terminates the module
void FileWriter::terminateModule()
{
    circuitFile << "endmodule" << endl;
}

// Function declares a net (input, output, or wire)
void FileWriter::declareNet(wire* net)
{
    // Declare net type
    if (net->type == WireType::INPUT)
    {
        circuitFile << "    input ";
    }
    else if (net->type == WireType::OUTPUT)
    {
        circuitFile << "    output ";
    }
    else
    {
        circuitFile << "    wire ";
    }

    // Declare net sign
    if (net->sign)
    {
        circuitFile << "signed ";
    }

    // Declare net range
    if (net->width > 1)
    {
        circuitFile << "[" << net->width - 1 << ":0] ";
    }

    // Terminate with a semicolon
    circuitFile << net->name << ";" << endl;
}

// Function declares inputs
void FileWriter::declareInputs()
{
    // Loop through all the wires in the data manager
    for (wire*& currWire : data_manager->wires)
    {
        // Declare all inputs
        if (currWire->type == WireType::INPUT)
        {
            declareNet(currWire);
        }
    }

    // Add a line of whitespace after the inputs
    circuitFile << endl;
}

// Function declares outputs
void FileWriter::declareOutputs()
{
    // Loop through all the wires in the data manager
    for (wire*& currWire : data_manager->wires)
    {
        // Declare all outputs
        if (currWire->type == WireType::OUTPUT)
        {
            declareNet(currWire);
        }
    }

    // Add a line of whitespace after the outputs
    circuitFile << endl;
}

// Function declares wires
void FileWriter::declareWires()
{
    // Loop through all the wires in the data manager
    for (wire*& currWire : data_manager->wires)
    {
        // Declare all wires and registers. Registers are declared as wires
        // because they are driven from a component instead of an always block
        if ((currWire->type == WireType::WIRE) || (currWire->type == WireType::REGISTER))
        {
            declareNet(currWire);
        }
    }

    // Add a line of whitespace after the wires
    circuitFile << endl;
}

// Function declares cast components
void FileWriter::declareCasts()
{
    // Flag specifies whether at least one cast component exists
    bool atLeastOneCast = false;

    // Loop through all components in the data manager
    for (component*& currComponent : data_manager->components)
    {
        // If component is a cast
        if (currComponent->type == ComponentType::CAST)
        {
            // Update flag
            atLeastOneCast = true;

            // Get input and output of each cast
            wire* output = currComponent->outputs["out"]->connection;
            wire* input = currComponent->inputs["in"]->connection;

            // Create assign statement for cast
            circuitFile << "    assign " << output->name << " = ";

            // Handle sign change
            if (output->sign != input->sign)
            {
                if (output->sign)
                {
                    circuitFile << "$signed(";
                }
                else
                {
                    circuitFile << "$unsigned(";
                }
            }

            // Handle width change
            if (output->width != input->width)
            {
                // Smaller width to larger width
                if (output->width > input->width)
                {
                    // Determine the number of bits to add
                    int numBitsToAdd = output->width - input->width;

                    // Sign extend
                    if (input->sign)
                    {
                        circuitFile << "{{" << numBitsToAdd << "{" << input->name << "[" << input->width - 1 << "]}}, " << input->name << "}";
                    }
                    // Zero pad
                    else
                    {
                        circuitFile << "{{" << numBitsToAdd << "{1'b0}}, " << input->name << "}";
                    }
                }
                // Larger width to smaller width
                else
                {
                    circuitFile << input->name << "[" << output->width-1 << ":0]";
                }
            }
            // No width change
            else
            {
                circuitFile << input->name;
            }

            // Terminate sign change
            if (output->sign != input->sign)
            {
                circuitFile << ");" << endl;
            }
            else
            {
                circuitFile << ";" << endl;
            }
        }
    }

    // Only add a newline if there is at least one cast
    if (atLeastOneCast)
    {
        circuitFile << endl;
    }
}

// Function declares components
void FileWriter::declareComponents()
{
    // Loop for each component in the data manager
    for (component*& currComponent : data_manager->components)
    {
        // Ignore cast components
        if (currComponent->type != ComponentType::CAST)
        {
            // Get string for component type
            string componentType = ComponentTypeToStr(currComponent->type);
            
            // Append 'S' for signed components
            if (currComponent->sign)
            {
                componentType = "S" + componentType;
            }

            // Declare component
            circuitFile << "    " << componentType;
            circuitFile << " #(.DATAWIDTH(" << currComponent->width << ")) ";
            circuitFile << currComponent->name << "(";
            
            // Declare each of the component inputs
            auto begin = currComponent->inputs.begin();
            auto end = currComponent->inputs.end();
            bool firstWire = true;
            for (auto it = begin; it != end; ++it)
            {
                if (!firstWire)
                {
                    circuitFile << ", ";
                }
                firstWire = false;
                circuitFile << "." << it->first << "(" << it->second->connection->name << ")";
            }

            // Declare each of the component outputs
            begin = currComponent->outputs.begin();
            end = currComponent->outputs.end();
            for (auto it = begin; it != end; ++it)
            {
                if (!firstWire)
                {
                    circuitFile << ", ";
                }
                firstWire = false;
                circuitFile << "." << it->first << "(" << it->second->connection->name << ")";
            }
            circuitFile << ");" << endl;
        }
    }

    // Terminate section with a newline
    circuitFile << endl;
}

// Function writes the output file
int FileWriter::run(string filePath)
{
    // Attempt to open file
    int fileStatus = openFile(filePath);

    // Return with error if unable to open file
    if (fileStatus != 0)
    {
        return fileStatus;
    }
    determineModuleName(filePath);
    declareTimescale();
    declareModule();
    declareInputs();
    declareOutputs();
    declareWires();
    declareCasts();
    declareComponents();
    terminateModule();
    closeFile ();
    return 0;
}

// Function opens the output verilog file for writing
int FileWriter::openFile(string filePath)
{
    // Attempt to open file
    circuitFile.open(filePath);

    // Return with an error message if unable to open file
    if (!circuitFile.is_open())
    {
        cout << "ERROR: Unable to open output file: \"" << filePath << "\". Ensure output directory exists.";
        return 1;
    }
    return 0;
}

// Function closes the output verilog file
void FileWriter::closeFile()
{
    circuitFile.close ();
}
} // namespace DataPathGen
