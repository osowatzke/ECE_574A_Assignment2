#include "WireParser.h"

#include "ParserTypes.h"

#include <iostream>
#include <regex>
#include <vector>

namespace Parser
{
    WireParser::WireParser(ParserDataManager* data_manager)
        : data_manager(data_manager) {}
    void WireParser::parse_line(string line)
    {
        regex multi_wire_pattern{"^\\s*(input|output|wire|register)\\s*(U?)Int(\\d+)\\s+(.*)$"};
        smatch multi_wire_match;
        regex_match(line, multi_wire_match, multi_wire_pattern);
        if (!multi_wire_match.empty())
        {
            WireType type = get_wire_type(multi_wire_match.str(1));
            bool sign = (multi_wire_match.str(2) != "U");
            int width = stoi(multi_wire_match.str(3));
            string wire_substring = multi_wire_match.str(4);
            regex wire_pattern{"(\\w+)\\s*(?:,|$)"};
            sregex_iterator start = sregex_iterator(wire_substring.begin(), wire_substring.end(), wire_pattern);
            sregex_iterator end = sregex_iterator();
            for (sregex_iterator i = start; i != end; ++i)
            {
                smatch wire_match = *i;
                wire* new_wire = new wire;
                new_wire->name = wire_match.str(1);
                new_wire->type = type;
                new_wire->sign = sign;
                new_wire->width = width;
                data_manager->wires.push_back(new_wire);
            }
        }
    }
    WireType WireParser::get_wire_type(string str)
    {
        WireType type;
        if (str == "input")
        {
            type = WireType::INPUT;
        }
        else if (str == "output")
        {
            type = WireType::OUTPUT;
        }
        else if (str == "wire")
        {
            type = WireType::WIRE;
        }
        else
        {
            type = WireType::REGISTER;
        }
        return type;
    }
    void WireParser::parse_lines(vector<string> lines)
    {
        for (size_t i = 0; i < lines.size(); ++i)
        {
            parse_line(lines[i]);
        }
        for (size_t i = 0; i < data_manager->wires.size(); ++i)
        {
            cout << data_manager->wires[i]->name << endl;
        }
    }
}