#ifndef GUARD_ImplicitComponentGenerator_h
#define GUARD_ImplicitComponentGenerator_h

#include "ParserDataManager.h"
#include "ParserTypes.h"

#include <vector>

namespace Parser
{
class ImplicitComponentGenerator
{
    public:
        void run(ParserDataManager* data_manager);
    private:
        void cast_wire(ParserDataManager* data_manager, wire* wire);
        void generate_implicit_registers(ParserDataManager* data_manager);
        void generate_implicit_casts(ParserDataManager* data_manager);
        void name_all_components(ParserDataManager* data_manager);
        string ComponentNameToString(ComponentType type);
};
} // namespace Parser

#endif // GUARD_ImplicitComponentGenerator_h
