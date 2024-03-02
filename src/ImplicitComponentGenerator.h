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
        void fix_single_bit_wires();
        void cast_wire(wire* wire);
        void generate_implicit_registers();
        void generate_implicit_casts();
        void name_all_components();
        string ComponentNameToString(ComponentType type);
};
} // namespace DataPathGen

#endif // GUARD_ImplicitComponentGenerator_h
