#include <regex>

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
}