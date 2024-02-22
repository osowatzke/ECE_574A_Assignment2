#include "ParserDataManager.h"
#include "ParserTypes.h"

namespace Parser
{
    wire* ParserDataManager::find_wire(string name)
    {
        for (size_t i = 0; i < wires.size(); ++i)
        {
            if (wires[i]->name == name)
            {
                return wires[i];
            }
        }
        return NULL;
    }
    ParserDataManager::~ParserDataManager()
    {
        for (size_t i = 0; i < wires.size(); ++i)
        {
            delete wires[i];
        }
        for (size_t i = 0; i < components.size(); ++i)
        {
            auto begin = components[i]->inputs.begin();
            auto end = components[i]->inputs.end();
            for (auto j = begin; j != end; ++j)
            {
                delete j->second;
            }
            delete components[i];
        }
    }
} // namespace Parser