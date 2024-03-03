#include "ImplicitComponentGenerator.h"
#include "ComponentParser.h"
#include "DataManager.h"
#include "DataTypes.h"
#include <iostream>

using namespace std;

namespace DataPathGen
{
    void ImplicitComponentGenerator::run()
    {
        generate_implicit_casts();
        generate_implicit_registers();
        name_all_components();
    }

    void ImplicitComponentGenerator::cast_wire(wire* currWire)
    {
        // Iterate through all destination components of the current wire
        for (component*& currComponent : currWire->dest) {
            // Check if there is a width or sign mismatch between the source wire and the current destination component
            if ((currWire->type != WireType::INPUT) && ((currWire->src->width != currComponent->width) || (currWire->src->sign != currComponent->sign))) {
                bool foundViableCast = false;
                for (component*& tempComponent : currWire->dest) {
                    if ((tempComponent->type == ComponentType::CAST) && ((tempComponent->sign == currComponent->sign) && (tempComponent->type == currComponent->type))) {
                        for (pair<string, port*> currPort : currComponent->inputs) {
                            if (currPort.second->connection == currWire) {
                                // TODO: Verify that this is able to redirect the input port of a component to a cast component and remove dest connection of currWire to tempComponent
                                currPort.second->connection->dest.push_back(tempComponent);
                                break;
                            }
                        }
                        break;
                    }
                }
                
                if (!foundViableCast) {
                    // Create a new wire for the cast operation if it doesn't already exist
                    wire* newWire = data_manager->create_wire(("cast_" + currWire->name), WireType::WIRE, currComponent->width, currComponent->sign);
                    
                    // Create a new cast component
                    component* newCastComponent = data_manager->create_cast(currComponent->width, currComponent->sign, {currWire, newWire});
                    
                    // Connect the new cast component to the new wire
                    newWire->src = newCastComponent;
                    // Connect the current destination component to the new wire
                    newWire->dest.push_back(currComponent);

                    // Update the destination component to point to the new cast component
                    currComponent = newCastComponent;
                }
            }
        }
    }

    void ImplicitComponentGenerator::generate_implicit_registers()
    {
        // Iterate through all wires in the data manager
        for (wire*& currWire : data_manager->wires) {
            // Check if the wire is of type OUTPUT or REGISTER and its source is not a register
            if (((currWire->type == WireType::OUTPUT) || (currWire->type == WireType::REGISTER)) && (currWire->src->type != ComponentType::REG)) {
                // Create a new wire for the cast operation if it doesn't already exist
                wire* newWire = data_manager->create_wire(data_manager->get_unique_name("reg_" + currWire->src->name), WireType::WIRE, currWire->src->width, currWire->src->sign);
                
                // Connect the new wire to the source wire
                newWire->src = currWire->src;

                // Create a new reg component
                component* newRegComponent = data_manager->create_reg(currWire->src->width, currWire->src->sign, {currWire, newWire});

                // Connect the new register component to the new wire
                newWire->dest.push_back(newRegComponent);

                // Update the source wire to point to the new register component
                currWire->src = newRegComponent;
            }
        }
    }

    void ImplicitComponentGenerator::generate_implicit_casts()
    {
        // Iterate through all wires in the data manager
        for (wire*& currWire : data_manager->wires) {
            ImplicitComponentGenerator::cast_wire(currWire);
        }
    }

    void ImplicitComponentGenerator::name_all_components()
    {
        // Iterate through all components in the data manager
        for (component*& currComponent : data_manager->components) {
            currComponent->name = data_manager->get_unique_name(ComponentTypeToStr(currComponent->type));
        }
    }

    void ImplicitComponentGenerator::generate_univeral_input_wires()
    {
        data_manager->create_wire("clk", WireType::INPUT, 1, 0);
        data_manager->create_wire("rst", WireType::INPUT, 1, 0);
    }
} // namespace Parser
