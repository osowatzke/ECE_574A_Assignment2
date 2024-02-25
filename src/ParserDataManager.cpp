#include "ParserDataManager.h"
#include "ParserTypes.h"

#include <regex>

namespace Parser
{
    wire* ParserDataManager::create_wire(string name)
    {
        // Create a new wire with the unique name
        wire* newWire = new wire;
        newWire->name = get_unique_name(name);

        // Add the new wire to the vector of wires
        wires.push_back(newWire);

        // Return a pointer to the new wire
        return newWire;
    }

    component* ParserDataManager::create_component(string name, ComponentType type, int width, bool sign)
    {
        component* newComponent = new component;
        newComponent->name = get_unique_name(name);
        newComponent->type = type;
        newComponent->width = width;
        newComponent->sign = sign;

        // Add the new component to the vector of components
        components.push_back(newComponent);

        return newComponent;
    }

    string ParserDataManager::get_unique_name(string name)
    {
        // Constructing the regular expression pattern to match names like "name_#"
        std::regex pattern(name + "_\\d+");
        
        // Count of wires with the same base name
        int nameInc = 1;
        bool isMatch = true;

        // Loop through existing wires to find a unique name
        
        while(isMatch) {
            isMatch = false;
            for (size_t i = 0; i < wires.size(); ++i)
            {
                // If the name matches the base name or the pattern, increment the counter
                if ((wires[i]->name == name) || ((nameInc > 0) && (wires[i]->name == (name + "_" + to_string(nameInc)))))
                {
                    isMatch = true;
                    nameInc += 1;
                }
            }
        }

        // If there are wires with the same base name, append an incremented number
        if (nameInc > 0) {
            name += "_" + std::to_string(nameInc);
        }

        return name;
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
