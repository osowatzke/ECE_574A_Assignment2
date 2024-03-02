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
        void run();
        ImplicitComponentGenerator(DataManager* data_manager) {
            this->data_manager = data_manager;
        }
    private:
        DataManager* data_manager;
        void cast_wire(wire* wire);
        void generate_implicit_registers();
        void generate_implicit_casts();
        void name_all_components();
        void generate_univeral_input_wires();
        string ComponentNameToString(ComponentType type);
};
} // namespace Parser

#endif // GUARD_ImplicitComponentGenerator_h
