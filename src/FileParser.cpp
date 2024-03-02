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

namespace Parser{

FileParser::FileParser(DataManager* data_manager)
    : data_manager(data_manager)
    , wire_parser(WireParser(data_manager))
    , component_parser(ComponentParser(data_manager)) {}

void FileParser::run(string file_path)
{
    read_lines(file_path);
    remove_comments();
    parse_wires();
    parse_components();
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

void FileParser::parse_components()
{
    component_parser.parse_lines(lines);
}

} // namespace Parser