#ifndef GUARD_WireParser_h
#define GUARD_WireParser_h

#include "ParserDataManager.h"

#include <string>
#include <vector>

namespace Parser
{
    class WireParser
    {
        public:
            WireParser(ParserDataManager* data_manager);
            void parse_line(string line);
            void parse_lines(vector<string> lines);
        private:
            ParserDataManager* data_manager;
            static WireType get_wire_type(string str);
    };
} // namespace Parser

#endif // GUARD_WireParser_h