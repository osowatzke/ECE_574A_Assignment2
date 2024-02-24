#include "ParserDataManager.h"

#include "ComponentParser.h"

#include "ParserTypes.h"

#include <iostream>
#include <vector>

using namespace std;

namespace Parser
{
ComponentParser::ComponentParser(ParserDataManager* data_manager)
    : data_manager(data_manager)
{
    ParserTableEntry REG_ENTRY;
    REG_ENTRY.r = REG_REGEX;
    REG_ENTRY.func = &ComponentParser::parse_reg;
    ParserTableEntry ADD_ENTRY;
    ADD_ENTRY.r = ADD_REGEX;
    ADD_ENTRY.func = &ComponentParser::parse_add;
    ParserTableEntry SUB_ENTRY;
    SUB_ENTRY.r = SUB_REGEX;
    SUB_ENTRY.func = &ComponentParser::parse_sub;
    ParserTableEntry MUL_ENTRY;
    MUL_ENTRY.r = MUL_REGEX;
    MUL_ENTRY.func = &ComponentParser::parse_mul;
    ParserTableEntry GT_ENTRY;
    GT_ENTRY.r = GT_REGEX;
    GT_ENTRY.func = &ComponentParser::parse_gt;
    ParserTableEntry LT_ENTRY;
    LT_ENTRY.r = LT_REGEX;
    LT_ENTRY.func = &ComponentParser::parse_lt;
    ParserTableEntry EQ_ENTRY;
    EQ_ENTRY.r = EQ_REGEX;
    EQ_ENTRY.func = &ComponentParser::parse_eq;
    ParserTableEntry MUX2x1_ENTRY;
    MUX2x1_ENTRY.r = MUX2x1_REGEX;
    MUX2x1_ENTRY.func = &ComponentParser::parse_mux2x1;
    ParserTableEntry SHL_ENTRY;
    SHL_ENTRY.r = SHL_REGEX;
    SHL_ENTRY.func = &ComponentParser::parse_shl;
    ParserTableEntry SHR_ENTRY;
    SHR_ENTRY.r = SHR_REGEX;
    SHR_ENTRY.func = &ComponentParser::parse_shr;
    ParserTableEntry DIV_ENTRY;
    DIV_ENTRY.r = DIV_REGEX;
    DIV_ENTRY.func = &ComponentParser::parse_div;
    ParserTableEntry MOD_ENTRY;
    MOD_ENTRY.r = MOD_REGEX;
    MOD_ENTRY.func = &ComponentParser::parse_mod;
    ParserTableEntry INC_ENTRY;
    INC_ENTRY.r = INC_REGEX;
    INC_ENTRY.func = &ComponentParser::parse_inc;
    ParserTableEntry DEC_ENTRY;
    DEC_ENTRY.r = DEC_REGEX;
    DEC_ENTRY.func = &ComponentParser::parse_dec;
    PARSER_TABLE = {REG_ENTRY, ADD_ENTRY, SUB_ENTRY, MUL_ENTRY,
            GT_ENTRY, LT_ENTRY, EQ_ENTRY, MUX2x1_ENTRY, SHR_ENTRY,
            SHL_ENTRY, DIV_ENTRY, MOD_ENTRY, INC_ENTRY, DEC_ENTRY};
}
void ComponentParser::parse_line(string line)
{
    for (size_t i = 0; i < PARSER_TABLE.size(); ++i)
    {
        smatch match;
        regex_match(line, match, PARSER_TABLE[i].r);
        if (!match.empty())
        {
            component* new_component = new component;
            vector <wire*> wires;
            for (size_t j = 1; j < match.size(); ++j)
            {
                wires.push_back(data_manager->find_wire(match.str(j)));
                if (j == 1)
                {
                    wires[j-1]->src.push_back(new_component);
                }
                else
                {
                    wires[j-1]->dest.push_back(new_component);
                }
            }
            data_manager->components.push_back(new_component);
            (*this.*(PARSER_TABLE[i].func))(wires);
            return;
        }
    }
}
void ComponentParser::parse_lines(vector<string> lines)
{
    for (size_t i = 0; i < lines.size(); ++i)
    {
        parse_line(lines[i]);
    }

    // TODO: Check this loop that iterates through all components, their ports, and the wires between components to update the port and wire widths if they are too small.
    for (component* currComponent : data_manager->components) {
        int componentWidth = currComponent->width;
        for (auto& input : currComponent->inputs) {
            if (input.second->width < componentWidth) {
                input.second->width = componentWidth;
                input.second->connection->width = componentWidth;
            }
        }
        for (auto& output : currComponent->outputs) {
            if (((output.second->connection->type == WireType::OUTPUT) || (output.second->connection->type == REGISTER))) {
                for (auto& currWireDest : output.second->connection->dest) {
                    if (currWireDest->type != ComponentType::REG) {
                        // Create a new register component
                        component* newComponent = new component;
                        newComponent->type = ComponentType::REG;
                        newComponent->width = output.second->connection->width;

                        // Create a new wire for the register
                        wire* newWire = data_manager->create_wire(output.second->connection->name);
                        newWire->type = output.second->connection->type;

                        // Create new input and output ports for the register component
                        port* newPort = new port;
                        newPort->width = output.second->connection->width;
                        newPort->connection = newWire;
                        newComponent->inputs.insert({"input", newPort});

                        newPort = new port;
                        newPort->width = output.second->connection->width;
                        newPort->connection = output.second->connection;
                        newComponent->outputs.insert({"output", newPort});

                        // Add the new component to the data manager
                        components.push_back(newComponent);
                    }
                }
            }
        }
    }
}
void ComponentParser::parse_reg(vector<wire*> wires)
{
    component* reg = data_manager->components.back();
    reg->type = ComponentType::REG;
    port* d = new port;
    port* q = new port;
    d->connection = wires[1];
    q->connection = wires[0];
    reg->width = d->connection->width;
    d->width = reg->width;
    q->width = reg->width;
    reg->inputs["d"] = d;
    reg->outputs["q"] = q;
}
void ComponentParser::parse_add(vector<wire*> wires)
{
    component* add = data_manager->components.back();
    add->type = ComponentType::ADD;
    port* a = new port;
    port* b = new port;
    port* sum = new port;
    a->connection = wires[1];
    b->connection = wires[2];
    sum->connection = wires[0];
    add->width = sum->connection->width;
    a->width = add->width;
    b->width = add->width;
    sum->width = add->width;
    add->inputs["a"] = a;
    add->inputs["b"] = b;
    add->outputs["sum"] = sum;
}
void ComponentParser::parse_sub(vector<wire*> wires)
{
    component* sub = data_manager->components.back();    
    sub->type = ComponentType::SUB;
    port* a = new port;
    port* b = new port;
    port* diff = new port;
    a->connection = wires[1];
    b->connection = wires[2];
    diff->connection = wires[0];
    sub->width = diff->connection->width;
    a->width = sub->width;
    b->width = sub->width;
    diff->width = sub->width;
    sub->inputs["a"] = a;
    sub->inputs["b"] = b;
    sub->outputs["diff"] = diff;
}
void ComponentParser::parse_mul(vector<wire*> wires)
{
    component* mul = data_manager->components.back();
    mul->type = ComponentType::MUL;
    port* a = new port;
    port* b = new port;
    port* prod = new port;   
    a->connection = wires[1];
    b->connection = wires[2];
    prod->connection = wires[0];
    mul->width = prod->connection->width;
    a->width = mul->width;
    b->width = mul->width;
    prod->width = mul->width;
    mul->inputs["a"] = a;
    mul->inputs["b"] = b;
    mul->outputs["prod"] = prod;
}
void ComponentParser::parse_gt(vector<wire*> wires)
{
    component* comp = data_manager->components.back();
    comp->type = ComponentType::COMP;
    port* a = new port;
    port* b = new port;
    port* gt = new port;
    port* lt = new port;
    port* eq = new port;
    a->connection = wires[1];
    b->connection = wires[2];
    gt->connection = wires[0];
    lt->connection = NULL;
    eq->connection = NULL;
    comp->width = max(a->connection->width, b->connection->width);
    a->width = comp->width;
    b->width = comp->width;
    gt->width = 1;
    lt->width = 1;
    eq->width = 1;
    comp->inputs["a"] = a;
    comp->inputs["b"] = b;
    comp->outputs["gt"] = gt;
    comp->outputs["lt"] = lt;
    comp->outputs["eq"] = eq;
}

void ComponentParser::parse_lt(vector<wire*> wires)
{
    component* comp = data_manager->components.back();
    comp->type = ComponentType::COMP;
    port* a = new port;
    port* b = new port;
    port* gt = new port;
    port* lt = new port;
    port* eq = new port;
    a->connection = wires[1];
    b->connection = wires[2];
    gt->connection = NULL;
    lt->connection = wires[0];
    eq->connection = NULL;
    comp->width = max(a->connection->width, b->connection->width);
    a->width = comp->width;
    b->width = comp->width;
    gt->width = 1;
    lt->width = 1;
    eq->width = 1;
    comp->inputs["a"] = a;
    comp->inputs["b"] = b;
    comp->outputs["gt"] = gt;
    comp->outputs["lt"] = lt;
    comp->outputs["eq"] = eq;
}

void ComponentParser::parse_eq(vector<wire*> wires)
{
    component* comp = data_manager->components.back();
    comp->type = ComponentType::COMP;
    port* a = new port;
    port* b = new port;
    port* gt = new port;
    port* lt = new port;
    port* eq = new port;
    a->connection = wires[1];
    b->connection = wires[2];
    gt->connection = NULL;
    lt->connection = NULL;
    eq->connection = wires[0];
    comp->width = max(a->connection->width, b->connection->width);
    a->width = comp->width;
    b->width = comp->width;
    gt->width = 1;
    lt->width = 1;
    eq->width = 1;
    comp->inputs["a"] = a;
    comp->inputs["b"] = b;
    comp->outputs["gt"] = gt;
    comp->outputs["lt"] = lt;
    comp->outputs["eq"] = eq;
}

void ComponentParser::parse_mux2x1(vector<wire*> wires)
{
    component* mux2x1 = data_manager->components.back();
    mux2x1->type = ComponentType::MUX2x1;
    port* sel = new port;
    port* b = new port;
    port* a = new port;
    port* d = new port;
    sel->connection = wires[1];   
    b->connection = wires[2];
    a->connection = wires[3];
    d->connection = wires[0];
    mux2x1->width = d->connection->width;
    sel->width = 1;
    b->width = mux2x1->width;
    a->width = mux2x1->width;
    d->width = mux2x1->width;
    mux2x1->inputs["sel"] = sel;
    mux2x1->inputs["b"] = b;
    mux2x1->inputs["a"] = a;
    mux2x1->outputs["d"] = d;
}
void ComponentParser::parse_shr(vector<wire*> wires)
{
    component* shr = data_manager->components.back();
    shr->type = ComponentType::SHR;
    port* a = new port;
    port* sh_amt = new port;
    port* d = new port;
    a->connection = wires[1];
    sh_amt->connection = wires[2];
    d->connection = wires[0];
    shr->width = d->connection->width;
    a->width = shr->width;
    sh_amt->width = shr->width;
    d->width = shr->width;
    shr->inputs["a"] = a;
    shr->inputs["sh_amt"] = sh_amt;
    shr->outputs["d"] = d;
}
void ComponentParser::parse_shl(vector<wire*> wires)
{
    component* shl = data_manager->components.back();
    shl->type = ComponentType::SHL;
    port* a = new port;
    port* sh_amt = new port;
    port* d = new port;
    a->connection = wires[1];
    sh_amt->connection = wires[2];
    d->connection = wires[0];
    shl->width = d->connection->width;
    a->width = shl->width;
    sh_amt->width = shl->width;
    d->width = shl->width;
    shl->inputs["a"] = a;
    shl->inputs["sh_amt"] = sh_amt;
    shl->outputs["d"] = d;
}
void ComponentParser::parse_div(vector<wire*> wires)
{
    component* div = data_manager->components.back();
    div->type = ComponentType::DIV;
    port* a = new port;
    port* b = new port;
    port* qout = new port;
    a->connection = wires[1];
    b->connection = wires[2];
    qout->connection = wires[0];
    div->width = qout->connection->width;
    a->width = div->width;
    b->width = div->width;
    qout->width = div->width;
    div->inputs["a"] = a;
    div->inputs["b"] = b;
    div->outputs["qout"] = qout;
}
void ComponentParser::parse_mod(vector<wire*> wires)
{
    component* mod = data_manager->components.back();
    mod->type = ComponentType::MOD;
    port* a = new port;
    port* b = new port;
    port* rem = new port;
    a->connection = wires[1];
    b->connection = wires[2];
    rem->connection = wires[0];
    mod->width = rem->connection->width;
    a->width = mod->width;
    b->width = mod->width;
    rem->width = mod->width;
    mod->inputs["a"] = a;
    mod->inputs["b"] = b;
    mod->outputs["rem"] = rem;
}
void ComponentParser::parse_inc(vector<wire*> wires)
{
    component* inc = data_manager->components.back();
    inc->type = ComponentType::INC;
    port* a = new port;
    port* d = new port;
    a->connection = wires[1];
    d->connection = wires[0];
    inc->width = d->connection->width;
    a->width = inc->width;
    d->width = inc->width;
    inc->inputs["a"] = a;
    inc->outputs["d"] = d;
}
void ComponentParser::parse_dec(vector<wire*> wires)
{
    component* dec = data_manager->components.back();
    dec->type = ComponentType::DEC;
    port* a = new port;
    port* d = new port;
    a->connection = wires[1];
    d->connection = wires[0];
    dec->width = d->connection->width;
    a->width = dec->width;
    d->width = dec->width;
    dec->inputs["a"] = a;
    dec->outputs["d"] = d;
}
} // namespace Parser
} // namespace Parser
