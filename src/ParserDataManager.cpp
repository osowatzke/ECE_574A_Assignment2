#include "ParserDataManager.h"
#include "ParserTypes.h"

#include <regex>

namespace Parser
{
    // TODO: Check that the create_wire function produces proper output (namely that there aren't aren't duplicate names).
    wire* ParserDataManager::create_wire(string name)
    {
        // Constructing the regular expression pattern to match names like "name_#"
        std::regex pattern(name + "_\\d+");

        // Count of wires with the same base name
        int nameInc = 0;

        // Loop through existing wires to find a unique name
        for (size_t i = 0; i < wires.size(); ++i)
        {
            // If the name matches the base name or the pattern, increment the counter
            if ((wires[i]->name == name) || (std::regex_search(wires[i]->name, pattern)))
            {
                nameInc += 1;
            }
        }

        // If there are wires with the same base name, append an incremented number
        if (nameInc > 0) {
            name += "_" + std::to_string(nameInc);
        }

        // Create a new wire with the unique name
        wire* newWire = new wire;
        newWire->name = name;

        // Add the new wire to the vector of wires
        wires.push_back(newWire);

        // Return a pointer to the new wire
        return newWire;
    }

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
