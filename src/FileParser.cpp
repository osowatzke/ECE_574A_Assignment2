#include "FileParser.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

namespace FileParsing{

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

void FileParser::read_inputs()
{
    read_signals(inputs, "input");
}

void FileParser::read_outputs()
{
    read_signals(outputs, "output");
}

void FileParser::read_wires()
{
    read_signals(wires, "wire");
}

void FileParser::read_registers()
{
    read_signals(registers, "register");
}

void FileParser::read_signals(vector<signal> & signals, string signal_type)
{

    regex signal_pattern{"^" + signal_type + " (U?)Int(\\d+)\\s+(.*)"};
    smatch signal_match;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        regex_match(lines[i], signal_match, signal_pattern);
        if (!signal_match.empty())
        {
            bool sign = (signal_match.str(1) != "U");
            int width = stoi(signal_match.str(2));
            string signal_substring = signal_match.str(3);
            regex signal_subpattern{"(\\w)\\s*(?:,|$)"};
            sregex_iterator start = sregex_iterator(signal_substring.begin(), signal_substring.end(), signal_subpattern);
            sregex_iterator end = sregex_iterator();
            for (sregex_iterator j = start; j != end; ++j)
            {
                smatch signal_submatch = *j;
                signal sig;
                sig.sign = sign;
                sig.width = width;
                sig.name = signal_submatch.str(1);
                signals.push_back(sig);
            }
        }
    }
}

}

void print_signals(const vector<FileParsing::signal> & signals, string signal_type)
{
    cout << signal_type << ":" << endl;
    for (size_t i = 0; i < signals.size(); ++i)
    {
        cout << "\t" << signals[i].name << ":" << endl;
        cout << "\t\tsign = " << signals[i].sign << endl;
        cout << "\t\twidth = " << signals[i].width << endl;
    }
}

int main(int argc, char** argv)
{
    FileParsing::FileParser file_parser;
    file_parser.read_lines(argv[1]);
    file_parser.read_inputs();
    file_parser.read_outputs();
    file_parser.read_wires();
    file_parser.read_outputs();
    print_signals(file_parser.inputs,    "Inputs");
    print_signals(file_parser.outputs,   "Outputs");
    print_signals(file_parser.wires,     "Wires");
    print_signals(file_parser.registers, "Registers");
    return 0;
}