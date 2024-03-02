#ifndef GUARD_FileParser_h
#define GUARD_FileParser_h

#include "ImplicitComponentGenerator.h"
#include "ComponentParser.h"
#include "DataManager.h"
#include "DataTypes.h"
#include "WireParser.h"

#include <string>
#include <vector>

using namespace std;

namespace Parser
{
    class FileParser{
        public:
            FileParser(DataManager* data_manager);
            void run(string file_path);
        private:            
            DataManager* data_manager;
            WireParser wire_parser;
            ComponentParser component_parser;
            vector<string> lines;
            int read_lines(string file_path);
            void remove_comments();
            void parse_wires();
            void parse_components();
    };
} // namespace Parser

#endif //GUARD_FileParser_h