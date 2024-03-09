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

FileParser::FileParser(DataManager* data_manager)
    : data_manager(data_manager)
    , wire_parser(WireParser(data_manager))
    , component_parser(ComponentParser(data_manager)) {}

int FileParser::run(string file_path)
{
    int retVal = read_lines(file_path);
    if (retVal)
    {
        return retVal;
    }
    remove_comments();
    parse_wires();
    retVal = parse_components();
    return retVal;
}

int FileParser::read_lines(string file_path)
{
    ifstream file (file_path);
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
        return 0;
    }
    cout << "ERROR: Unable to open file " << file_path << endl;
    return 1;
}

void FileParser::remove_comments()
{
    regex comment_pattern{"^(.*)//(?:.*)$"};
    smatch comment_match;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        regex_match(lines[i], comment_match, comment_pattern);
        if(!comment_match.empty())
        {
            lines[i] = comment_match.str(1);
        }
    }
}

void FileParser::parse_wires()
{
    wire_parser.parse_lines(lines);
}

int FileParser::parse_components()
{
    return component_parser.parse_lines(lines);
}

} // namespace DataPathGen