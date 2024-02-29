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
        void run(DataManager* data_manager);
    private:
        void cast_wire(DataManager* data_manager, wire* wire);
        void generate_implicit_registers(DataManager* data_manager);
        void generate_implicit_casts(DataManager* data_manager);
        void name_all_components(DataManager* data_manager);
        string ComponentNameToString(ComponentType type);
};
} // namespace Parser

#endif // GUARD_ImplicitComponentGenerator_h
