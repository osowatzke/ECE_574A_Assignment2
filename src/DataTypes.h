#ifndef GUARD_DataTypes_h
#define GUARD_DataTypes_h

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace DataPathGen
{
struct component;
enum WireType{INPUT,OUTPUT,WIRE,REGISTER};
struct wire
{
    string name;
    WireType type;
    int width;
    bool sign;
    component* src;
    vector<component*> dest;
};
struct port
{
    int width;
    int sign;
    wire* connection;
};
enum ComponentType{REG,ADD,SUB,MUL,COMP,MUX2x1,SHR,SHL,DIV,MOD,INC,DEC,CAST};
struct component
{
    string name;
    ComponentType type;
    int width;
    int sign;
    map<string, port*> inputs;
    map<string, port*> outputs;
};
inline string ComponentTypeToStr(ComponentType type)
{
    switch (type) {
        case ComponentType::REG:
            return "REG";
        case ComponentType::ADD:
            return "ADD";
        case ComponentType::SUB:
            return "SUB";
        case ComponentType::MUL:
            return "MUL";
        case ComponentType::COMP:
            return "COMP";
        case ComponentType::MUX2x1:
            return "MUX2x1";
        case ComponentType::SHR:
            return "SHR";
        case ComponentType::SHL:
            return "SHL";
        case ComponentType::DIV:
            return "DIV";
        case ComponentType::MOD:
            return "MOD";
        case ComponentType::INC:
            return "INC";
        case ComponentType::DEC:
            return "DEC";
        case ComponentType::CAST:
            return "CAST";
    }
    return "UNKNOWN"; // Return "UNKNOWN" for invalid enumerator
}
} // namespace DataPathGen

#endif // GUARD_DataTypes_h