#include "WireParser.h"

#include "DataManager.h"
#include "DataTypes.h"

#include <regex>
#include <vector>

using namespace std;

namespace DataPathGen
{
    // Class constructor provides access to shared data manager
    WireParser::WireParser(DataManager* data_manager)
        : data_manager(data_manager) {}

    // Function parses wires from a line of the input netlist file
    void WireParser::parse_line(string line)
    {
        // Regular expression to match wire
        // Wires are the union of all inputs, outputs, wires, and registers
        regex multi_wire_pattern{"^\\s*(input|output|wire|register)\\s*(U?)Int(\\d+)\\s+(.*)$"};
        smatch multi_wire_match;
        regex_match(line, multi_wire_match, multi_wire_pattern);

        // If line matches regular expression  
        if (!multi_wire_match.empty())
        {
            // Extract sign and width of wires
            WireType type = get_wire_type(multi_wire_match.str(1));
            bool sign = (multi_wire_match.str(2) != "U");
            int width = stoi(multi_wire_match.str(3));

            // Regular expression to match individual wire names
            string wire_substring = multi_wire_match.str(4);
            regex wire_pattern{"(\\w+)\\s*(?:,|$)"};

            // Create a wire structure for each name that is found
            sregex_iterator start = sregex_iterator(wire_substring.begin(), wire_substring.end(), wire_pattern);
            sregex_iterator end = sregex_iterator();
            for (sregex_iterator i = start; i != end; ++i)
            {
                smatch wire_match = *i;
                wire* new_wire = new wire;
                new_wire->name = wire_match.str(1);
                new_wire->type = type;
                new_wire->width = width;

                // Force single bit wires to be unsigned
                if (new_wire->width > 1)
                {
                    new_wire->sign = sign;
                }
                else
                {
                    new_wire->sign = false;
                }             
                new_wire->src = NULL;

                // Update wires in data manager
                data_manager->wires.push_back(new_wire);
            }
        }
    }

    // Function determines wire type from string
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

    // Function parses all lines in the input netlist file for wires
    void WireParser::parse_lines(vector<string> lines)
    {
        for (string& line : lines)
        {
            parse_line(line);
        }
    }
} // namespace DataPathGen