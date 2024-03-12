#include "FileParser.h"

#include "ImplicitComponentGenerator.h"
#include "ComponentParser.h"
#include "DataManager.h"
#include "DataTypes.h"
#include "WireParser.h"

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

using namespace std;

namespace DataPathGen{

// Class constructor provides access to shared data manager
FileParser::FileParser(DataManager* data_manager)
    : data_manager(data_manager)
    , wire_parser(WireParser(data_manager))
    , component_parser(ComponentParser(data_manager)) {}

// Function runs the file parser
int FileParser::run(string file_path)
{
    // Attempt to read lines from input file
    int retVal = read_lines(file_path);

    // Return if read fails (i.e. can't open input file)
    if (retVal)
    {
        return retVal;
    }

    // Strip comments from lines
    remove_comments();

    // Parse wires
    parse_wires();

    // Parse components
    retVal = parse_components();

    // Return if component parsing fails
    if (retVal)
    {
        return retVal;
    }

    // Validate circuit (i.e. ensure circuit is non empty)
    retVal = validate_circuit();
    return retVal;
}

// Function reads lines from the input file
int FileParser::read_lines(string file_path)
{
    // Attempt to open file
    ifstream file (file_path);

    // If file can be openend
    if (file.is_open())
    {
        string line;
        
        // Read lines from input file
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
        return 0;
    }

    // Return with error if file cannot be read
    cout << "ERROR: Unable to open file " << file_path << endl;
    return 1;
}

// Function reads lines from input file
void FileParser::remove_comments()
{
    // Regular expression to match comments
    regex comment_pattern{"^(.*)//(?:.*)$"};
    smatch comment_match;

    // Go through each line of the input file
    for (string& line : lines)
    {
        // Attempt to match regular expression
        // Will match if there is a comment
        regex_match(line, comment_match, comment_pattern);

        // If there is a match, strip comment from file
        if(!comment_match.empty())
        {
            line = comment_match.str(1);
        }
    }
}

// Function parses wires in input nelist file
void FileParser::parse_wires()
{
    wire_parser.parse_lines(lines);
}

// Function parses components in input netlist file
int FileParser::parse_components()
{
    return component_parser.parse_lines(lines);
}

// Function validates whether circuit is valid
int FileParser::validate_circuit()
{
    // If circuit is empty, issue an error message and return a non-zero value
    if ((data_manager->wires.size() == 0) && (data_manager->components.size() == 0))
    {
        cout << "ERROR: Empty Netlist Provided" << endl;
        return 1;
    }

    // Return 0 for valid circuit
    return 0;
}

} // namespace DataPathGen