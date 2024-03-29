#include "ImplicitComponentGenerator.h"
#include "ComponentParser.h"
#include "DataManager.h"
#include "DataTypes.h"

#include <iostream>

using namespace std;

namespace DataPathGen
{
// Class constructor provides access to shared data manager
ImplicitComponentGenerator::ImplicitComponentGenerator(DataManager* data_manager)
    : data_manager(data_manager) {}

// Function runs the implicit component generator
void ImplicitComponentGenerator::run()
{
    collapse_comparators();
    generate_implicit_casts();
    generate_implicit_registers();
    name_all_components();
    create_implicit_input_wires();
}

// Function determines whether two components share the same inputs
bool ImplicitComponentGenerator::shared_inputs(component* a, component* b)
{
    // Initialize starting and ending interators
    auto aBegin = a->inputs.begin();
    auto bBegin = b->inputs.begin();
    auto aEnd = a->inputs.end();
    auto bEnd = b->inputs.end();

    // Determine if inputs are matched
    for (auto i = aBegin; i != aEnd; ++i)
    {
        bool inputMatch = false;
        for (auto j = bBegin; j != bEnd; ++j)
        {
            if (i->second->connection == j->second->connection)
            {
                inputMatch = true;
            }
        }
        if (!inputMatch)
        {
            return false;
        }
    }
    return true;
}

// Function combines comparators
void ImplicitComponentGenerator::combine_comparators(component* a, component* b)
{
    // Append output of b to output of a
    string outputKey = b->outputs.begin()->first;
    port* outputValue = b->outputs.begin()->second;
    a->outputs[outputKey] = outputValue;
    outputValue->parent = a;

    // Loop through all the input ports of b
    auto begin = b->inputs.begin();
    auto end = b->inputs.end();
    for (auto it = begin; it != end; ++it)
    {
        port* input = it->second;
        wire* srcWire = input->connection;

        // Remove the wire connection to b's input ports
        auto portBegin = srcWire->dest.begin();
        auto portEnd = srcWire->dest.end();
        for (auto portIt = portBegin; portIt != portEnd; ++portIt)
        {
            if (input == *portIt)
            {
                srcWire->dest.erase(portIt);
                delete input;
                break;
            }
        }
    }

    // Delete component
    auto componentBegin = data_manager->components.begin();
    auto componentEnd = data_manager->components.end();
    for (auto it = componentBegin; it != componentEnd; ++it)
    {
        if (*it == b)
        {
            data_manager->components.erase(it);
            delete b;
            break;
        }
    }
}

// Function collapses multiple comparators which have the same inputs to a
// single comparator which uses multiple output ports
void ImplicitComponentGenerator::collapse_comparators()
{
    // Get all the comparator objects
    vector<component*> comparators;
    for (component*& currComponent : data_manager->components)
    {
        if (currComponent->type == ComponentType::COMP)
        {
            comparators.push_back(currComponent);
        }
    }

    // Loop through each comparator
    vector<bool> valid(comparators.size(), true);
    for (size_t i = 0; i < comparators.size(); ++i)
    {
        // Only consider components that are still valid
        if (valid[i])
        {
            for (size_t j = i + 1; j < comparators.size(); ++ j)
            {
                // Only consider components that are still valid
                if (valid[j])
                {
                    // If both comparators share common inputs
                    if (shared_inputs(comparators[i], comparators[j]))
                    {
                        // Combine comparators
                        combine_comparators(comparators[i], comparators[j]);

                        // Update valid to reflect deleted comparator
                        valid[j] = false;
                    }
                }
            }
        }
    }
}

// Function casts wire to correct type
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
        wire* newWire = create_wire(newWireName, WireType::WIRE, currWire->src->width, currWire->src->sign);
        newWire->src = currWire->src;
        currWire->src->connection = newWire;
        existingWires.push_back(newWire);

        // Connect new wire to current wire
        component* castComponent = create_cast(currWire->src->width, currWire->src->sign, {newWire, currWire});
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
            wire* newWire = create_wire(newWireName, WireType::WIRE, currPort->width, currPort->sign);
            newWire->dest.push_back(currPort);
            currPort->connection = newWire;
            existingWires.push_back(newWire);

            // Connect new wire to current wire
            component* newComponent = create_cast(newWire->width, newWire->sign, {currWire, newWire});
            currWire->dest.push_back(newComponent->inputs["in"]);
            newWire->src = newComponent->outputs["out"];
        }
    }
}

// Function generates implicit registers
void ImplicitComponentGenerator::generate_implicit_registers()
{
    // Iterate through all wires in the data manager
    for (wire*& currWire : data_manager->wires)
    {
        // Check if the wire is of type OUTPUT or REGISTER and its source is not a register
        if (((currWire->type == WireType::OUTPUT) || (currWire->type == WireType::REGISTER)) && (currWire->src->parent->type != ComponentType::REG)) {
            
            // Create a new wire for the cast operation if it doesn't already exist
            wire* newWire = create_wire(currWire->name + "wire", WireType::WIRE, currWire->src->width, currWire->src->sign);
            
            // Connect the new wire to the source wire
            currWire->src->connection = newWire;
            newWire->src = currWire->src;

            // Create a new reg component
            component* newRegComponent = create_reg(currWire->src->width, currWire->src->sign, {newWire, currWire});

            // Connect the new register component to the new wire
            newWire->dest.push_back(newRegComponent->inputs["d"]);

            // Update the source wire to point to the new register component
            currWire->src = newRegComponent->outputs["q"];
        }
    }
}

// FUnction generates implicit casts
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

// Function names all components
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

// Helper function for creating a new wire
wire* ImplicitComponentGenerator::create_wire(string name, WireType type, int width, bool sign)
{
    // Create a new wire with the unique name
    wire* newWire = new wire;
    newWire->name = data_manager->get_unique_wire_name(name);

    // Set the properties of the new wire
    newWire->type = type;
    newWire->width = width;
    newWire->sign = sign;

    // Add the new wire to the vector of wires
    data_manager->wires.push_back(newWire);

    // Return a pointer to the new wire
    return newWire;
}

// Helper function for create a register
component* ImplicitComponentGenerator::create_reg(int width, bool sign, vector<wire*> wires)
{
    component* reg = new component;
    data_manager->components.push_back(reg);
    reg->type = ComponentType::REG;
    port* d = new port;
    port* q = new port;
    d->connection = wires[0];
    q->connection = wires[1];
    d->parent = reg;
    q->parent = reg;
    reg->width = width;
    reg->sign = sign;
    d->width = reg->width;
    q->width = reg->width;
    reg->inputs["d"] = d;
    reg->outputs["q"] = q;

    return reg;
}

// Helper function for create a cast
component* ImplicitComponentGenerator::create_cast(int width, bool sign, vector<wire*> wires)
{
    component* cast = new component;
    data_manager->components.push_back(cast);
    cast->type = ComponentType::CAST;
    port* in = new port;
    port* out = new port;
    in->connection = wires[0];
    out->connection = wires[1];
    in->parent = cast;
    out->parent = cast;
    cast->width = width;
    cast->sign = sign;
    in->width = in->connection->width;
    out->width = out->connection->width;
    cast->inputs["in"] = in;
    cast->outputs["out"] = out;

    return cast;
}

// Function creates implicit input wires (ex: Clk and Rst)
void ImplicitComponentGenerator::create_implicit_input_wires() {

    // Create clk wire
    wire* clkWire = create_wire("Clk", WireType::INPUT, 1, 0);

    // Create rst wire
    wire* rstWire = create_wire("Rst", WireType::INPUT, 1, 0);

    // Check and change the names of any other wire with the name clk or rst
    for (wire* currWire : data_manager->wires) {
        if ((currWire->name == "Clk") && (currWire != clkWire)) {
            currWire->name = data_manager->get_unique_wire_name(currWire->name);
        }

        if ((currWire->name == "Rst") && (currWire != rstWire)) {
            currWire->name = data_manager->get_unique_wire_name(currWire->name);
        }
    }

    // Add clk and rst as variables to reg components
    for (component* currComponent : data_manager->components) {
        if (currComponent->type == ComponentType::REG) {
            // Connect the clk wire to the reg
            currComponent->inputs["Clk"] = new port;
            currComponent->inputs["Clk"]->sign = false;
            currComponent->inputs["Clk"]->width = 1;
            currComponent->inputs["Clk"]->connection = clkWire;
            clkWire->dest.push_back(currComponent->inputs["Clk"]);

            // Connect the rst wire to the reg
            currComponent->inputs["Rst"] = new port;
            currComponent->inputs["Rst"]->sign = false;
            currComponent->inputs["Rst"]->width = 1;
            currComponent->inputs["Rst"]->connection = rstWire;
            rstWire->dest.push_back(currComponent->inputs["Rst"]);
        }
    }
}
} // namespace DataPathGen
