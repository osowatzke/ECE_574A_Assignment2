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
        // cout << "Creating Cast for " << currWire->name << endl;
        // Iterate through all destination components of the current wire
        for (size_t i = 0; i < currWire->dest.size(); ++ i){ //component*& currComponent : currWire->dest) {
            port * currPort = currWire->dest[i];
            // Check if there is a width or sign mismatch between the source wire and the current destination component
            if ((currWire->type != WireType::INPUT) && ((currWire->width != currPort->width) || (currWire->sign != currPort->sign))) {

                // Create a new wire for the cast operation if it doesn't already exist
                cout << "Trying to create a wire" << endl;
                wire* newWire = data_manager->create_wire("cast_" + currWire->name);
                cout << data_manager->wires.size() << endl;
                // Set the properties of the new wire
                newWire->sign = currPort->sign;
                newWire->type = WireType::WIRE;
                newWire->width = currPort->width;

                // cout << "Created Wire " << newWire->name << endl;
                
               // cout << currWire->dest.size() << endl;
                
                // Create a new cast component
                component* newCastComponent = data_manager->create_cast(currPort->width, currPort->sign, {currWire, newWire});
                
                // Connect the new cast component to the new wire
                newWire->src = newCastComponent->outputs["out"];
                // Connect the current destination component to the new wire
                newWire->dest.push_back(currPort);

                // delete(newWire);

                // Update the destination component to point to the new cast component
                // currComponent = newCastComponent;
                currWire->dest[i] = newCastComponent->inputs["in"];

                //cout << currWire->dest.size() << endl;
            }
            cout << "Finished another loop iteration" << endl;
        }
        cout << "Finished the Fucking loop" << endl;
    }

    void ImplicitComponentGenerator::generate_implicit_registers()
    {
        // Iterate through all wires in the data manager
        for (wire*& currWire : data_manager->wires) {
            // Check if the wire is of type OUTPUT or REGISTER and its source is not a register
            if (((currWire->type == WireType::OUTPUT) || (currWire->type == WireType::REGISTER)) && (currWire->src->parent->type != ComponentType::REG)) {
                // Create a new wire for the cast operation if it doesn't already exist
                wire* newWire = data_manager->create_wire("reg_" + currWire->name);
                // Set the properties of the new wire
                newWire->sign = currWire->src->sign;
                newWire->type = WireType::WIRE;
                newWire->width = currWire->src->width;
                
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
        // Iterate through all wires in the data manager
        vector<wire*> initialWires;
        copy(data_manager->wires.begin(), data_manager->wires.end(), back_inserter(initialWires));        
        cout << data_manager->wires.size() << endl;
        for (wire* & currWire : initialWires) {
            ImplicitComponentGenerator::cast_wire(currWire);
        }
    }

    void ImplicitComponentGenerator::name_all_components()
    {
        int componentCount[NUM_COMPONENT_TYPES][2] = {0};
        // Iterate through all components in the data manager
        for (component*& currComponent : data_manager->components) {
            int idx = ComponentTypeToInt(currComponent->type);
            currComponent->name = ComponentTypeToStr(currComponent->type);
            if (currComponent->sign = 1) {
                currComponent->name = "S" + currComponent->name;
                currComponent->name += to_string(componentCount[idx][1]);
                ++componentCount[idx][1];
            } else {
                currComponent->name += to_string(componentCount[idx][0]);
                ++componentCount[idx][0];
            }
        }
    }
} // namespace Parser
