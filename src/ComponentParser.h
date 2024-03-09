#ifndef GUARD_ComponentParser_h
#define GUARD_ComponentParser_h

#include "DataTypes.h"
#include "DataManager.h"

#include <regex>
#include <vector>

using namespace std;

namespace DataPathGen
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
        void create_reg(vector<port*> ports);
        void create_add(vector<port*> ports);
        void create_sub(vector<port*> ports);
        void create_mul(vector<port*> ports);
        void create_comp_gt(vector<port*> ports);
        void create_comp_lt(vector<port*> ports);
        void create_comp_eq(vector<port*> ports);
        void create_mux2x1(vector<port*> ports);
        void create_shr(vector<port*> ports);
        void create_shl(vector<port*> ports);
        void create_div(vector<port*> ports);
        void create_mod(vector<port*> ports);
        void create_inc(vector<port*> ports);
        void create_dec(vector<port*> ports);
};
} // namespace DataPathGen

#endif // GUARD_ComponentParser_h