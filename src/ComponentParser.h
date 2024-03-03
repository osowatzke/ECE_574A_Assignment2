#ifndef GUARD_ComponentParser_h
#define GUARD_ComponentParser_h

#include "DataTypes.h"
#include "DataManager.h"

#include <regex>
#include <vector>

using namespace DataPathGen;
using namespace std;

namespace Parser
{

class ComponentParser
{
    public:
        vector<component*> components;
        ComponentParser(DataManager* data_manager);
        int parse_lines(vector<string> lines);
    private:
        DataManager* data_manager;
        vector <wire*> undefined_wires;
        int parse_line(string line);
        void parse_reg(vector<port*> ports);
        void parse_add(vector<port*> ports);
        void parse_sub(vector<port*> ports);
        void parse_mul(vector<port*> ports);
        void parse_gt(vector<port*> ports);
        void parse_lt(vector<port*> ports);
        void parse_eq(vector<port*> ports);
        void parse_mux2x1(vector<port*> ports);
        void parse_shr(vector<port*> ports);
        void parse_shl(vector<port*> ports);
        void parse_div(vector<port*> ports);
        void parse_mod(vector<port*> ports);
        void parse_inc(vector<port*> ports);
        void parse_dec(vector<port*> ports);
};
} // namespace Parser

#endif // GUARD_ComponentParser_h