#ifndef GUARD_WireParser_h
#define GUARD_WireParser_h

#include "DataManager.h"
#include "DataTypes.h"

#include <string>
#include <vector>

using namespace std;

namespace DataPathGen
{
    class WireParser
    {
        public:
            WireParser(DataManager* data_manager);
            void parse_line(string line);
            void parse_lines(vector<string> lines);
        private:
            DataManager* data_manager;
            static WireType get_wire_type(string str);
    };
} // namespace DataPathGen

#endif // GUARD_WireParser_h