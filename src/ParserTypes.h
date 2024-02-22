#ifndef GUARD_ParserTypes_h
#define GUARD_ParserTypes_h

#include <map>
#include <string>
#include <vector>

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
    vector<component*> src;
    vector<component*> dest;
};
struct port
{
    int width;
    wire* connection;
};
enum ComponentType{REG,ADD,SUB,MUL,COMP,MUX2x1,SHR,SHL,DIV,MOD,INC,DEC};
struct component
{
    string name;
    ComponentType type;
    int width;
    map<string, port*> inputs;
    map<string, port*> outputs;
};
} // namespace Parser

#endif // GUARD_ParserTypes_h