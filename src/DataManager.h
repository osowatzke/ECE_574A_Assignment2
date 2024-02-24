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
        wire* find_wire(string name);
        void print_wires();
        void print_components();
};
} // namespace DataPathGen

#endif // GUARD_DataManager_h