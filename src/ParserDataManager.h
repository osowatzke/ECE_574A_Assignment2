#ifndef GUARD_ParserDataManager_h
#define GUARD_ParserDataManager_h

#include "ParserTypes.h"

#include <vector>

namespace Parser
{
class ParserDataManager
{
    public:
        vector<wire*> wires;
        vector<component*> components;
        ~ParserDataManager();
        wire* find_wire(string name);
};
}
#endif // GUARD_ParserDataManager_h