#include "DataManager.h"
#include "DataTypes.h"

#include <iostream>

namespace DataPathGen
{

    string DataManager::get_unique_name(string name)
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

wire* DataManager::find_wire(string name)
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

void DataManager::print_wires()
{
    cout << "WIRES:" << endl;
    for (size_t i = 0; i < wires.size(); ++i)
    {
        cout << "\t" << wires[i]->name << endl;
        cout << "\t\twidth = " << wires[i]->width << endl;
        cout << "\t\tsigned = " << wires[i]->sign << endl;
    }
}

void DataManager::print_components()
{
    cout << "COMPONENTS:" << endl;
    for (size_t i = 0; i < components.size(); ++i)
    {
        cout << "\t" << components[i]->name << endl;
        cout << "\t\ttype = " << ComponentTypeToStr(components[i]->type) << endl;
        cout << "\t\tsigned = " << components[i]->sign << endl;
        cout << "\t\tdatawidth = " << components[i]->width << endl;
        auto begin = components[i]->inputs.begin();
        auto end = components[i]->inputs.end();
        for (auto j = begin; j != end; ++j)
        {
            if (j->second->connection != NULL)
            {
                cout << "\t\t" << j->first << " = " << j->second->connection->name << endl;
            }
        }
        begin = components[i]->outputs.begin();
        end = components[i]->outputs.end();
        for (auto j = begin; j != end; ++j)
        {
            if (j->second->connection != NULL)
            {
                cout << "\t\t" << j->first << " = " << j->second->connection->name << endl;
            }
        }
    }
}

DataManager::~DataManager()
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
        begin = components[i]->outputs.begin();
        end = components[i]->outputs.end();
        for (auto j = begin; j != end; ++j)
        {
            delete j->second;
        }
        delete components[i];
    }
}
} // namespace DataPathGen