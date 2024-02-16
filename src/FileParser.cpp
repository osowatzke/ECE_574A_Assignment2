#include "FileParser.h"

#include "ComponentParser.h"

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

using namespace std;

namespace Parser{

FileParser::FileParser()
    : data_manager(ParserDataManager())
    , wire_parser(WireParser(&data_manager))
    , component_parser(ComponentParser(&data_manager)) {}

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

void FileParser::print_wires()
{
    cout << "WIRES:" << endl;
    for (size_t i = 0; i < data_manager.wires.size(); ++i)
    {
        cout << "\t" << data_manager.wires[i]->name << endl;
        cout << "\t\twidth = " << data_manager.wires[i]->width << endl;
        cout << "\t\tsigned = " << data_manager.wires[i]->sign << endl;
    }
}

void FileParser::print_components()
{
    cout << "COMPONENTS:" << endl;
    for (size_t i = 0; i < data_manager.components.size(); ++i)
    {
        cout << "\t" << get_component_type(data_manager.components[i]) << endl;
        auto begin = data_manager.components[i]->inputs.begin();
        auto end = data_manager.components[i]->inputs.end();
        for (auto j = begin; j != end; ++j)
        {
            if (j->second != NULL)
            {
                cout << "\t\t" << j->first << " = " << j->second->name << endl;
            }
        }
        begin = data_manager.components[i]->outputs.begin();
        end = data_manager.components[i]->outputs.end();
        for (auto j = begin; j != end; ++j)
        {
            if (j->second != NULL)
            {
                cout << "\t\t" << j->first << " = " << j->second->name << endl;
            }
        }
    }
}

string FileParser::get_component_type(component* comp)
{
    switch (comp->type)
    {
        case ComponentType::REG:
            return "REG";
        case ComponentType::ADD:
            return "ADD";
        case ComponentType::SUB:
            return "SUB";
        case ComponentType::MUL:
            return "MUL";
        case ComponentType::COMP:
            return "COMP";
        case ComponentType::MUX2x1:
            return "MUX2x1";
        case ComponentType::SHR:
            return "SHR";
        case ComponentType::SHL:
            return "SHL";
        case ComponentType::DIV:
            return "DIV";
        case ComponentType::MOD:
            return "MOD";
        case ComponentType::INC:
            return "INC";
        case ComponentType::DEC:
            return "DEC";
        default:
            return "";
    }
}
} // namespace Parser

int main(int argc, char** argv)
{
    Parser::FileParser file_parser;
    file_parser.read_lines(argv[1]);
    file_parser.remove_comments();
    file_parser.parse_wires();
    file_parser.parse_components();
    file_parser.print_wires();
    file_parser.print_components();
    return 0;
}