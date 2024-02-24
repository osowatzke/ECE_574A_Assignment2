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

const regex REG_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*$"};
const regex ADD_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*\\+\\s*(?!1)(\\w+)\\s*$"};
const regex SUB_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*-\\s*(?!1)(\\w+)\\s*$"};
const regex MUL_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*\\*\\s*(\\w+)\\s*$"};
const regex GT_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*>\\s*(\\w+)\\s*$"};
const regex LT_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*<\\s*(\\w+)\\s*$"};
const regex EQ_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*==\\s*(\\w+)\\s*$"};
const regex MUX2x1_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s\\?\\s*(\\w+)\\s*:\\s*(\\w+)\\s*$"};
const regex SHR_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*>>\\s*(\\w+)\\s*$"};
const regex SHL_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*<<\\s*(\\w+)\\s*$"};
const regex DIV_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*/\\s*(\\w+)\\s*$"};
const regex MOD_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*%\\s*(\\w+)\\s*$"};    
const regex INC_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*\\+\\s*1\\s*$"};
const regex DEC_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*-\\s*1\\s*$"};

class ComponentParser;

struct ParserTableEntry{
    regex r;
    void (ComponentParser::*func)(vector<wire*> wires);
};

class ComponentParser
{
    public:
        vector<component*> components;
        ComponentParser(DataManager* data_manager);
        void parse_line(string line);
        void parse_lines(vector<string> lines);
    private:
        DataManager* data_manager; 
        vector<ParserTableEntry> PARSER_TABLE;
        void parse_reg(vector<wire*> wires);
        void parse_add(vector<wire*> wires);
        void parse_sub(vector<wire*> wires);
        void parse_mul(vector<wire*> wires);
        void parse_gt(vector<wire*> wires);
        void parse_lt(vector<wire*> wires);
        void parse_eq(vector<wire*> wires);
        void parse_mux2x1(vector<wire*> wires);
        void parse_shr(vector<wire*> wires);
        void parse_shl(vector<wire*> wires);
        void parse_div(vector<wire*> wires);
        void parse_mod(vector<wire*> wires);
        void parse_inc(vector<wire*> wires);
        void parse_dec(vector<wire*> wires);
};
} // namespace Parser

#endif // GUARD_ComponentParser_h