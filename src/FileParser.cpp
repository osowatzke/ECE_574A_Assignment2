#include "FileParser.h"

#include "ComponentParser.h"

#include <fstream>
#include <iostream>
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

// void FileParser::read_inputs()
// {
//     read_signals(inputs, "input");
// }

// void FileParser::read_outputs()
// {
//     read_signals(outputs, "output");
// }

// void FileParser::read_wires()
// {
//     read_signals(wires, "wire");
// }

// void FileParser::read_registers()
// {
//     read_signals(registers, "register");
// }

// void FileParser::read_signals(vector<signal> & signals, string signal_type)
// {

//     regex signal_pattern{"^/s*" + signal_type + " (U?)Int(\\d+)\\s+(.*)$"};
//     smatch signal_match;
//     for (size_t i = 0; i < lines.size(); ++i)
//     {
//         regex_match(lines[i], signal_match, signal_pattern);
//         if (!signal_match.empty())
//         {
//             bool sign = (signal_match.str(1) != "U");
//             int width = stoi(signal_match.str(2));
//             string signal_substring = signal_match.str(3);
//             regex signal_subpattern{"(\\w)\\s*(?:,|$)"};
//             sregex_iterator start = sregex_iterator(signal_substring.begin(), signal_substring.end(), signal_subpattern);
//             sregex_iterator end = sregex_iterator();
//             for (sregex_iterator j = start; j != end; ++j)
//             {
//                 smatch signal_submatch = *j;
//                 signal sig;
//                 sig.sign = sign;
//                 sig.width = width;
//                 sig.name = signal_submatch.str(1);
//                 signals.push_back(sig);
//             }
//         }
//     }
// }

// void FileParser::read_operations()
// {
//     // struct opRegex {
//     //     string name;
//     //     regex r;
//     // };
//     // const regex REG{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*$"};
//     // const regex ADD{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*\\+\\s*(?!1)(\\w+)\\s*$"};
//     // const regex SUB{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*-\\s*(?!1)(\\w+)\\s*$"};
//     // const regex MUL{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*\\*\\s*(\\w+)\\s*$"};
//     // const regex COMP{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*(>|<|==)\\s*(\\w+)\\s*$"};
//     // const regex MUX2x1{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s\\?\\s*(\\w+)\\s*:\\s*(\\w+)\\s*$"};
//     // const regex SHR{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*>>\\s*(\\w+)\\s*$"};
//     // const regex SHL{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*<<\\s*(\\w+)\\s*$"};
//     // const regex DIV{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*/\\s*(\\w+)\\s*$"};
//     // const regex MOD{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*%\\s*(\\w+)\\s*$"};
//     // const vector<opRegex> REGEX_TABLE = {
//     //     {.name="REG", .r=REG},
//     //     {.name="ADD", .r=ADD},
//     //     {.name="SUB", .r=SUB},
//     //     {.name="MUL", .r=MUL},
//     //     {.name="COMP", .r=COMP},
//     //     {.name="MUX2x1", .r=MUX2x1},
//     //     {.name="SHR", .r=SHR},
//     //     {.name="SHL", .r=SHL}};
//     // smatch operation_match;
//     Parser::ComponentParser component_parser;
//     for (size_t i = 0; i < lines.size(); ++i)
//     {
//         component_parser.parse_line(lines[i]);
//         // for (size_t j = 0; j < REGEX_TABLE.size(); ++j)
//         // {
//         //     // cout << REGEX_TABLE[j].r.text << endl;
//         //     regex_match(lines[i], operation_match, REGEX_TABLE[j].r);
//         //     if (!operation_match.empty())
//         //     {
//         //         cout << operation_match.str(0) << endl;
//         //     }
//         // }
//     }
// }

}

/*void print_signals(const vector<FileParsing::signal> & signals, string signal_type)
{
    cout << signal_type << ":" << endl;
    for (size_t i = 0; i < signals.size(); ++i)
    {
        cout << "\t" << signals[i].name << ":" << endl;
        cout << "\t\tsign = " << signals[i].sign << endl;
        cout << "\t\twidth = " << signals[i].width << endl;
    }
}*/

int main(int argc, char** argv)
{
    Parser::FileParser file_parser;
    file_parser.read_lines(argv[1]);
    file_parser.remove_comments();
    file_parser.parse_wires();
    file_parser.print_wires();
    file_parser.parse_components();
    /*print_signals(file_parser.inputs,    "Inputs");
    print_signals(file_parser.outputs,   "Outputs");
    print_signals(file_parser.wires,     "Wires");
    print_signals(file_parser.registers, "Registers");*/
    return 0;
}