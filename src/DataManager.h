#ifndef GUARD_DataManager_h
#define GUARD_DataManager_h

#include "DataTypes.h"

#include <vector>

namespace DataPathGen
{
class DataManager
{
    public:
        vector<wire*> wires;
        vector<component*> components;
        ~DataManager();
        wire* create_wire(string name, WireType type, int width, bool sign);
        component* create_reg(int width, bool sign, vector<wire*> wires);
        component* create_cast(int width, bool sign, vector<wire*> wires);
        string get_unique_name(string name);
        wire* find_wire(string name);
        void print_wires();
        void print_components();
};
} // namespace DataPathGen

#endif // GUARD_DataManager_h