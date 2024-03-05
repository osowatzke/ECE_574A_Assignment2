#include "ImplicitComponentGenerator.h"
#include "ComponentParser.h"
#include "DataManager.h"
#include "DataTypes.h"

#include <iostream>

using namespace std;

namespace DataPathGen
{
    ImplicitComponentGenerator::ImplicitComponentGenerator(DataManager* data_manager)
        : data_manager(data_manager) {}

    void ImplicitComponentGenerator::run()
    {
        generate_implicit_casts();
        generate_implicit_registers();
        name_all_components();
    }

    void ImplicitComponentGenerator::cast_wire(wire* currWire)
    {
        // Update wire width to match source width if source exists and wire does not drive output
        if ((currWire->type != WireType::INPUT) && (currWire->type != WireType::OUTPUT))
        {
            currWire->width = currWire->src->width;
            currWire->sign  = currWire->src->sign;
        }

        // Keep track of existing wires
        vector<wire*> existingWires;
        existingWires.push_back(currWire);

        // Store destination ports of current wire and clear vector of destination ports
        vector<port*> destPorts = currWire->dest;
        currWire->dest = {};

        // Handle wire source different from wire
        if ((currWire->type != WireType::INPUT) && ((currWire->src->width != currWire->width) || (currWire->src->sign != currWire->sign)))
        {
            // Create new wire
            string newWireName = "cast_" + currWire->name;
            wire* newWire = data_manager->create_wire(newWireName, WireType::WIRE, currWire->src->width, currWire->src->sign);
            newWire->src = currWire->src;
            currWire->src->connection = newWire;
            existingWires.push_back(newWire);

            // Connect new wire to current wire
            component* castComponent = data_manager->create_cast(currWire->src->width, currWire->src->sign, {newWire, currWire});
            newWire->dest.push_back(castComponent->inputs["in"]);
            currWire->src = castComponent->outputs["out"];
        }

        // Loop through the set of initial destination ports
        for (port*& currPort : destPorts)
        {
            bool match = false;

            // Loop through all the existing wires
            for (wire*& existingWire : existingWires)
            {
                // Connect port to wire if matching wire exists
                if (!match && (currPort->width == existingWire->width) && (currPort->sign == existingWire->sign))
                {
                    existingWire->dest.push_back(currPort);
                    currPort->connection = existingWire;
                    match = true;
                }
            }

            // If there is no matching wire
            if (!match)
            {
                // Create new wire
                string newWireName = "cast_" + currWire->name;
                wire* newWire = data_manager->create_wire(newWireName, WireType::WIRE, currPort->width, currPort->sign);
                newWire->dest.push_back(currPort);
                currPort->connection = newWire;
                existingWires.push_back(newWire);
                
                // Connect new wire to current wire
                component* newComponent = data_manager->create_cast(newWire->width, newWire->sign, {currWire, newWire});
                currWire->dest.push_back(newComponent->inputs["in"]);
                newWire->src = newComponent->outputs["out"];
            }
        }
    }

    void ImplicitComponentGenerator::generate_implicit_registers()
    {
        // Iterate through all wires in the data manager
        for (wire*& currWire : data_manager->wires)
        {
            // Check if the wire is of type OUTPUT or REGISTER and its source is not a register
            if (((currWire->type == WireType::OUTPUT) || (currWire->type == WireType::REGISTER)) && (currWire->src->parent->type != ComponentType::REG)) {
                
                // Create a new wire for the cast operation if it doesn't already exist
                wire* newWire = data_manager->create_wire("reg_" + currWire->name, WireType::WIRE, currWire->src->width, currWire->src->sign);
                
                // Connect the new wire to the source wire
                newWire->src = currWire->src;

                // Create a new reg component
                component* newRegComponent = data_manager->create_reg(currWire->src->width, currWire->src->sign, {currWire, newWire});

                // Connect the new register component to the new wire
                newWire->dest.push_back(newRegComponent->inputs["d"]);

                // Update the source wire to point to the new register component
                currWire->src = newRegComponent->outputs["q"];
            }
        }
    }

    void ImplicitComponentGenerator::generate_implicit_casts()
    {
        // Create a copy of the initial wires in the data manager
        // The address of dynamically allocated vectors can change as elements are added
        vector<wire*> initialWires;
        copy(data_manager->wires.begin(), data_manager->wires.end(), back_inserter(initialWires));
        
        // Iterate through all of the initial wires
        for (wire* & currWire : initialWires) {
            ImplicitComponentGenerator::cast_wire(currWire);
        }
    }

    void ImplicitComponentGenerator::name_all_components()
    {
        // Create an array of component counts for each component type
        int componentCount[NUM_COMPONENT_TYPES][2] = {0};

        // Iterate through all components in the data manager
        for (component*& currComponent : data_manager->components)
        {

            // Get index of component type
            int idx = ComponentTypeToInt(currComponent->type);

            // Get string for component type
            currComponent->name = ComponentTypeToStr(currComponent->type);

            // Set name and update count
            if (currComponent->sign) 
            {
                currComponent->name = "S" + currComponent->name;
                currComponent->name += to_string(componentCount[idx][1]);
                ++componentCount[idx][1];
            }
            else {
                currComponent->name += to_string(componentCount[idx][0]);
                ++componentCount[idx][0];
            }
        }
    }
} // namespace DataPathGen
