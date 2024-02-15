#ifndef GUARD_ParserTypes_h
#define GUARD_ParserTypes_h
#include <map>
#include <string>

using namespace std;

namespace Parser
{
struct component;
enum WireType{INPUT,OUTPUT,WIRE,REGISTER};
struct wire
{
    string name;
    WireType type;
    int width;
    bool sign;
    component* source;
    component* dest;
};
enum ComponentType{REG,ADD,SUB,MUL,COMP,MUX2x1,SHR,SHL,DIV,MOD,INC,DEC};
struct component
{
    string name;
    ComponentType type;
    int width;
    map<string, wire*> inputs;
    map<string, wire*> outputs;
};
}
#endif // GUARD_ParserTypes_h