#include "DataManager.h"
#include "DataTypes.h"

#include <iostream>

namespace DataPathGen
{

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
        cout << "\t" << ComponentTypeToStr(components[i]->type) << endl;
        cout << "\t\tDATAWIDTH = " << components[i]->width << endl;
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
        delete components[i];
    }
}
} // namespace DataPathGen