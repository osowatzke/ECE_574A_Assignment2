#include "DataManager.h"
#include "DataTypes.h"

#include <iostream>

namespace DataPathGen
{
    wire* DataManager::create_wire(string name)
    {
        // Create a new wire with the unique name
        wire* newWire = new wire;
        newWire->name = get_unique_name(name);

        // Add the new wire to the vector of wires
        wires.push_back(newWire);

        // Return a pointer to the new wire
        return newWire;
    }

    component* DataManager::create_reg(int width, bool sign, vector<wire*> wires)
    {
        component* reg = components.back();
        reg->type = ComponentType::REG;
        port* d = new port;
        port* q = new port;
        d->connection = wires[1];
        q->connection = wires[0];
        reg->width = width;
        reg->sign = sign;
        d->width = reg->width;
        q->width = reg->width;
        reg->inputs["d"] = d;
        reg->outputs["q"] = q;

        return reg;
    }

    component* DataManager::create_cast(int width, bool sign, vector<wire*> wires)
    {
        component* cast = components.back();
        cast->type = ComponentType::CAST;
        port* in = new port;
        port* out = new port;
        in->connection = wires[1];
        out->connection = wires[0];
        cast->width = width;
        cast->sign = sign;
        in->width = cast->width;
        out->width = cast->width;
        cast->inputs["in"] = in;
        cast->outputs["out"] = out;

        return cast;
    }

    string DataManager::get_unique_name(string name)
    {
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