#include "DataManager.h"
#include "DataTypes.h"

#include <iostream>

namespace DataPathGen
{
    // Function returns a unique wire name 
    string DataManager::get_unique_wire_name(string name)
    {
        // Count of wires with the same base name
        int nameInc = -1;
        bool isMatch = true;

        // Loop through existing wires to find a unique name
        while(isMatch) {
            isMatch = false;
            for (size_t i = 0; i < wires.size(); ++i)
            {
                // If the name matches the base name or the pattern, increment the counter
                if (((nameInc < 0) && (wires[i]->name == name)) || ((nameInc >= 0) && (wires[i]->name == (name + "_" + to_string(nameInc)))))
                {
                    isMatch = true;
                    nameInc += 1;
                }
            }
        }

        // If there are wires with the same base name, append an incremented number
        if (nameInc >= 0) {
            name += "_" + std::to_string(nameInc);
        }

        return name;
    }

// Function returns a pointer to a wire with the given name
wire* DataManager::find_wire(string name)
{
    // Loop through all the current wires
    for (wire*& currWire : wires)
    {
        // Return pointer to wire if name matches
        if (currWire->name == name)
        {
            return currWire;
        }
    }
    return NULL;
}

// Function prints wires in data manager
// Foundational debugging routine
void DataManager::print_wires()
{
    cout << "WIRES:" << endl;
    for (wire*& currWire : wires)
    {
        cout << "\t" << currWire->name << endl;
        cout << "\t\twidth = " << currWire->width << endl;
        cout << "\t\tsigned = " << currWire->sign << endl;
    }
}

// Function prints components in data manager
// Foundational debugging routine
void DataManager::print_components()
{
    cout << "COMPONENTS:" << endl;
    for (component*& currComponent : components)
    {
        cout << "\t" << currComponent->name << endl;
        cout << "\t\ttype = " << ComponentTypeToStr(currComponent->type) << endl;
        cout << "\t\tsigned = " << currComponent->sign << endl;
        cout << "\t\tdatawidth = " << currComponent->width << endl;

        // print input connections  
        auto begin = currComponent->inputs.begin();
        auto end = currComponent->inputs.end();
        for (auto it = begin; it != end; ++it)
        {
            cout << "\t\t" << it->first << " = " << it->second->connection->name << endl;
        }

        // print output connections
        begin = currComponent->outputs.begin();
        end = currComponent->outputs.end();
        for (auto it = begin; it != end; ++it)
        {
            cout << "\t\t" << it->first << " = " << it->second->connection->name << endl;
        }
    }
}

// Class destructor
DataManager::~DataManager()
{
    // Delete wires
    for (wire*& currWire: wires)
    {
        delete currWire;
    }

    // Delete components
    for (component* currComponent : components)
    {
        // Delete input ports
        auto begin = currComponent->inputs.begin();
        auto end = currComponent->inputs.end();
        for (auto it = begin; it != end; ++it)
        {
            delete it->second;
        }

        // Delete output ports
        begin = currComponent->outputs.begin();
        end = currComponent->outputs.end();
        for (auto it = begin; it != end; ++it)
        {
            delete it->second;
        }

        // Delete component
        delete currComponent;
    }
}
} // namespace DataPathGen