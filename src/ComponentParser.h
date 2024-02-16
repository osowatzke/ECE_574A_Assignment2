#ifndef GUARD_ComponentParser_h
#define GUARD_ComponentParser_h

#include "ParserTypes.h"
#include "ParserDataManager.h"

#include <regex>
#include <vector>

using namespace std;

namespace Parser
{

const regex REG_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*$"};
const regex ADD_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*\\+\\s*(?!1)(\\w+)\\s*$"};
const regex SUB_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*-\\s*(?!1)(\\w+)\\s*$"};
const regex MUL_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*\\*\\s*(\\w+)\\s*$"};
const regex COMP_REGEX{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*(>|<|==)\\s*(\\w+)\\s*$"};
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
    void (ComponentParser::*func)(smatch);
};

class ComponentParser
{
    public:
        vector<component*> components;
        ComponentParser(ParserDataManager* data_manager);
        void parse_line(string line);
        void parse_lines(vector<string> lines);
    private:
        ParserDataManager* data_manager; 
        vector<ParserTableEntry> PARSER_TABLE;
        void parse_reg(smatch match);
        void parse_add(smatch match);
        void parse_sub(smatch match);
        void parse_mul(smatch match);
        void parse_comp(smatch match);
        void parse_mux2x1(smatch match);
        void parse_shr(smatch match);
        void parse_shl(smatch match);
        void parse_div(smatch match);
        void parse_mod(smatch match);
        void parse_inc(smatch match);
        void parse_dec(smatch match);
};
} // namespace Parser

#endif // GUARD_ComponentParser_h