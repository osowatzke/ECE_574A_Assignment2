#ifndef GUARD_ImplicitComponentGenerator_h
#define GUARD_ImplicitComponentGenerator_h

#include "DataManager.h"
#include "DataTypes.h"

#include <vector>

namespace DataPathGen
{
class ImplicitComponentGenerator
{
    public:
        ImplicitComponentGenerator(DataManager* data_manager);
        void run();
    private:
        DataManager* data_manager;
        void cast_wire(wire* wire);
        void generate_implicit_registers();
        void generate_implicit_casts();
        void name_all_components();
        void generate_univeral_input_wires();
        string ComponentNameToString(ComponentType type);
        wire* create_wire(string name, WireType type, int width, bool sign);
        component* create_reg(int width, bool sign, vector<wire*> wires);
        component* create_cast(int width, bool sign, vector<wire*> wires);
        void create_implicit_input_wires();
};
} // namespace DataPathGen

#endif // GUARD_ImplicitComponentGenerator_h
