#include "ImplicitComponentGenerator.h"
#include "ParserDataManager.h"
#include "ParserTypes.h"

namespace Parser
{
    void ImplicitComponentGenerator::run(ParserDataManager* data_manager)
    {
        generate_implicit_casts(data_manager);
        generate_implicit_registers(data_manager);
        name_all_components(data_manager);
    }

    void ImplicitComponentGenerator::cast_wire(ParserDataManager* data_manager, wire* currWire)
    {
        // Iterate through all destination components of the current wire
        for (component*& currComponent : currWire->dest) {
            // Check if there is a width or sign mismatch between the source wire and the current destination component
            if ((currWire->src->width != currComponent->width) || (currWire->src->sign != currComponent->sign)) {
                // Create a new cast component
                component* newCastComponent = data_manager->create_component("cast", ComponentType::CAST, currComponent->width, currComponent->sign);

                // Create a new wire for the cast operation if it doesn't already exist
                wire* newWire = data_manager->create_wire(("cast_" + currWire->name));
                // Set the properties of the new wire
                newWire->sign = currComponent->sign;
                newWire->type = WireType::WIRE;
                newWire->width = currComponent->width;
                // Connect the new cast component to the new wire
                newWire->src = newCastComponent;
                // Connect the current destination component to the new wire
                newWire->dest.push_back(currComponent);

                // Update the destination component to point to the new cast component
                currComponent = newCastComponent;
            }
        }
    }

    void ImplicitComponentGenerator::generate_implicit_registers(ParserDataManager* data_manager)
    {
        // Iterate through all wires in the data manager
        for (wire*& currWire : data_manager->wires) {
            // Check if the wire is of type OUTPUT or REGISTER and its source is not a register
            if (((currWire->type == WireType::OUTPUT) || (currWire->type == WireType::REGISTER)) && (currWire->src->type != ComponentType::REG)) {
                // Create a new register component
                component* newRegComponent = data_manager->create_component("reg", ComponentType::REG, currWire->src->width, currWire->src->sign);

                // Create a new wire for the cast operation if it doesn't already exist
                wire* newWire = data_manager->create_wire(("cast_" + currWire->src->name));
                // Set the properties of the new wire
                newWire->sign = currWire->src->sign;
                newWire->type = WireType::WIRE;
                newWire->width = currWire->src->width;
                
                // Connect the new wire to the source wire
                newWire->src = currWire->src;
                // Connect the new register component to the new wire
                newWire->dest.push_back(newRegComponent);

                // Update the source wire to point to the new register component
                currWire->src = newRegComponent;
            }
        }
    }

    void ImplicitComponentGenerator::generate_implicit_casts(ParserDataManager* data_manager)
    {
        // Iterate through all wires in the data manager
        for (wire*& currWire : data_manager->wires) {
            ImplicitComponentGenerator::cast_wire(data_manager, currWire);
        }
    }

    void ImplicitComponentGenerator::name_all_components(ParserDataManager* data_manager)
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
