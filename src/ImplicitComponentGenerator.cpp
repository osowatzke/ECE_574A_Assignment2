#include "ImplicitComponentGenerator.h"
#include "ComponentParser.h"
#include "DataManager.h"
#include "DataTypes.h"

using namespace std;

namespace DataPathGen
{
    ImplicitComponentGenerator::ImplicitComponentGenerator(DataManager* data_manager)
        : data_manager(data_manager) {}

    void ImplicitComponentGenerator::run()
    {
        fix_single_bit_wires();
        generate_implicit_casts();
        generate_implicit_registers();
        name_all_components();
    }

    void ImplicitComponentGenerator::fix_single_bit_wires()
    {
        // Loop through all wires
        for (wire*& currWire : data_manager->wires)
        {
            // Ensure single bit wires are unsigned
            if (currWire->width == 1)
            {
                currWire->sign = 0;
            }
        }
    }

    void ImplicitComponentGenerator::cast_wire(wire* currWire)
    {
        // Iterate through all destination components of the current wire
        for (port*& currPort : currWire->dest)
        {
            // Check if there is a width or sign mismatch between the source wire and the current destination port
            if ((currWire->width != currPort->width) || (currWire->sign != currPort->sign)) {

                // Create a new wire for the cast operation
                wire* newWire = data_manager->create_wire(("cast_" + currWire->name), WireType::WIRE, currPort->width, currPort->sign);

                // Create a new cast component
                component* newCastComponent = data_manager->create_cast(currPort->width, currPort->sign, {currWire, newWire});
                
                // Connect the new cast component to the new wire
                newWire->src = newCastComponent->outputs["out"];

                // Connect the current destination component to the new wire
                newWire->dest.push_back(currPort);
                currPort->connection = newWire;

                // Update the destination port to point to the new cast component
                currPort = newCastComponent->inputs["in"];
            }
        }

        // Check the source of each wire for width or sign mismatch
        if ((currWire->type != WireType::INPUT) && ((currWire->width != currWire->src->width) || (currWire->sign != currWire->src->sign)))
        {

            // Create a new wire for the cast operation
            wire* newWire = data_manager->create_wire(("cast_" + currWire->name), WireType::WIRE, currWire->src->width, currWire->src->sign);

            // Create a new cast component
            component* newCastComponent = data_manager->create_cast(newWire->width, newWire->sign, {newWire, currWire});
            
            // Connect the new wire to the current wires source
            newWire->src = currWire->src;

            // Connect the new cast component to the new wire
            newWire->dest.push_back(newCastComponent->inputs["in"]);

            // Connect the source of the current wire to the cast compontent
            currWire->src = newCastComponent->outputs["out"];
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
            if (currComponent->sign = 1) 
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
