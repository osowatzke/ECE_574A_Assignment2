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
    void (ComponentParser::*func)(vector<port*> ports);
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