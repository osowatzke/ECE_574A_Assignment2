#include "ImplicitComponentGenerator.h"
#include "ComponentParser.h"
#include "DataManager.h"
#include "DataTypes.h"

namespace DataPathGen
{
    
    ImplicitComponentGenerator::ImplicitComponentGenerator(DataManager* data_manager)
        : data_manager(data_manager)
    {
    }
    
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
            if ((currWire->src->width != currComponent->width) || (currWire->src->sign != currComponent->sign)) {
                // Create a new wire for the cast operation if it doesn't already exist
                wire* newWire = data_manager->create_wire(data_manager->get_unique_name("cast_" + currWire->name));
                // Set the properties of the new wire
                newWire->sign = currComponent->sign;
                newWire->type = WireType::WIRE;
                newWire->width = currComponent->width;
                
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

    void ImplicitComponentGenerator::generate_implicit_registers()
    {
        // Iterate through all wires in the data manager
        for (wire*& currWire : data_manager->wires) {
            // Check if the wire is of type OUTPUT or REGISTER and its source is not a register
            if (((currWire->type == WireType::OUTPUT) || (currWire->type == WireType::REGISTER)) && (currWire->src->type != ComponentType::REG)) {
                // Create a new wire for the cast operation if it doesn't already exist
                wire* newWire = data_manager->create_wire(data_manager->get_unique_name("reg_" + currWire->src->name));
                // Set the properties of the new wire
                newWire->sign = currWire->src->sign;
                newWire->type = WireType::WIRE;
                newWire->width = currWire->src->width;
                
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
            currComponent->name = data_manager->get_unique_name(ComponentNameToString(currComponent->type));
        }
    }

    string ImplicitComponentGenerator::ComponentNameToString(ComponentType type)
    {
        switch (type) {
            case ComponentType::REG:
                return "REG";
            case ComponentType::ADD:
                return "ADD";
            case ComponentType::SUB:
                return "SUB";
            case ComponentType::MUL:
                return "MUL";
            case ComponentType::COMP:
                return "COMP";
            case ComponentType::MUX2x1:
                return "MUX2x1";
            case ComponentType::SHR:
                return "SHR";
            case ComponentType::SHL:
                return "SHL";
            case ComponentType::DIV:
                return "DIV";
            case ComponentType::MOD:
                return "MOD";
            case ComponentType::INC:
                return "INC";
            case ComponentType::DEC:
                return "DEC";
            case ComponentType::CAST:
                return "CAST";
        }
        return "UNKNOWN"; // Return "UNKNOWN" for invalid enumerator
    }
} // namespace Parser
