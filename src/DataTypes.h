#ifndef GUARD_DataTypes_h
#define GUARD_DataTypes_h

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace DataPathGen
{
struct port;
enum WireType{INPUT,OUTPUT,WIRE,REGISTER};
struct wire
{
    string name;
    WireType type;
    int width;
    bool sign;
    port* src;
    vector<port*> dest;
};
struct component;
struct port
{
    int width;
    bool sign;
    wire* connection;
    component* parent;
};
enum ComponentType{REG,ADD,SUB,MUL,COMP,MUX2x1,SHR,SHL,DIV,MOD,INC,DEC,CAST};
struct component
{
    string name;
    ComponentType type;
    int width;
    bool sign;
    map<string, port*> inputs;
    map<string, port*> outputs;
};
const int NUM_COMPONENT_TYPES = 13;
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
inline int ComponentTypeToInt(ComponentType type)
{
    switch (type) {
        case ComponentType::REG:
            return 0;
        case ComponentType::ADD:
            return 1;
        case ComponentType::SUB:
            return 2;
        case ComponentType::MUL:
            return 3;
        case ComponentType::COMP:
            return 4;
        case ComponentType::MUX2x1:
            return 5;
        case ComponentType::SHR:
            return 6;
        case ComponentType::SHL:
            return 7;
        case ComponentType::DIV:
            return 8;
        case ComponentType::MOD:
            return 9;
        case ComponentType::INC:
            return 10;
        case ComponentType::DEC:
            return 11;
        case ComponentType::CAST:
            return 12;
    }
    return -1; // Return -1 for invalid enumerator
}
} // namespace DataPathGen

#endif // GUARD_DataTypes_h