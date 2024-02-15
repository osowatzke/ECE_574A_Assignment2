#ifndef GUARD_FileParser_h
#define GUARD_FileParser_h

#include "ComponentParser.h"
#include "WireParser.h"

#include <string>
#include <vector>

using namespace std;

namespace Parser
{
    // struct signal {
    //     string name;
    //     int width;
    //     bool sign;
    // };

    class FileParser{
        public:
            FileParser();
            int read_lines(string file_path);
            void remove_comments();
            void parse_wires();
            void parse_components();
            void print_wires();
            void print_components();
        private:            
            ParserDataManager data_manager;
            WireParser wire_parser;
            ComponentParser component_parser;
            vector<string> lines;
            string get_component_type(component* comp);
    };

    // enum SignalType {Input, Output, Wire, Register};


}

#endif //GUARD_FileParser_h