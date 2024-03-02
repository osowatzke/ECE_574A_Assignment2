#include "ComponentParser.h"

#include "DataTypes.h"

#include <iostream>
#include <vector>

using namespace DataPathGen;
using namespace std;

namespace Parser
{
ComponentParser::ComponentParser(DataManager* data_manager)
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
            vector <port*> ports;
            for (size_t j = 1; j < match.size(); ++j)
            {
                ports.push_back(new port);
                ports[j-1]->parent = new_component;
                wire* wire_to_connect = data_manager->find_wire(match.str(j));
                if (j == 1)
                {
                    wire_to_connect->src = ports[j-1];
                }
                else
                {
                    wire_to_connect->dest.push_back(ports[j-1]);
                }
                ports[j-1]->connection = wire_to_connect;
            }
            data_manager->components.push_back(new_component);
            (*this.*(PARSER_TABLE[i].func))(ports);
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
}
void ComponentParser::parse_reg(vector<port*> ports)
{
    component* reg = data_manager->components.back();
    reg->type = ComponentType::REG;
    port* d = ports[1];
    port* q = ports[0];
    reg->width = q->connection->width;
    reg->sign = q->connection->sign;
    d->width = reg->width;
    d->sign = reg->sign;
    q->width = reg->width;
    q->sign = reg->sign;
    reg->inputs["d"] = d;
    reg->outputs["q"] = q;
}
void ComponentParser::parse_add(vector<port*> ports)
{
    component* add = data_manager->components.back();
    add->type = ComponentType::ADD;
    port* a = ports[1];
    port* b = ports[2];
    port* sum = ports[0];
    add->width = sum->connection->width;
    add->sign = sum->connection->sign;
    a->width = add->width;
    a->sign = add->sign;
    b->width = add->width;
    b->sign = add->sign;
    sum->width = add->width;
    sum->sign = add->sign;
    add->inputs["a"] = a;
    add->inputs["b"] = b;
    add->outputs["sum"] = sum;
}
void ComponentParser::parse_sub(vector<port*> ports)
{
    component* sub = data_manager->components.back();    
    sub->type = ComponentType::SUB;
    port* a = ports[1];
    port* b = ports[2];
    port* diff = ports[0];
    sub->width = diff->connection->width;
    sub->sign = diff->connection->sign;
    a->width = sub->width;
    a->sign = sub->sign;
    b->width = sub->width;
    b->sign = sub->sign;
    diff->width = sub->width;
    diff->sign = sub->sign;
    sub->inputs["a"] = a;
    sub->inputs["b"] = b;
    sub->outputs["diff"] = diff;
}
void ComponentParser::parse_mul(vector<port*> ports)
{
    component* mul = data_manager->components.back();
    mul->type = ComponentType::MUL;
    port* a = ports[1];
    port* b = ports[2];
    port* prod = ports[0];
    mul->width = prod->connection->width;
    mul->sign = prod->connection->sign;
    a->width = mul->width;
    a->sign = mul->sign;
    b->width = mul->width;
    b->sign = mul->sign;
    prod->width = mul->width;
    prod->sign = mul->sign;
    mul->inputs["a"] = a;
    mul->inputs["b"] = b;
    mul->outputs["prod"] = prod;
}
void ComponentParser::parse_gt(vector<port*> ports)
{
    component* comp = data_manager->components.back();
    comp->type = ComponentType::COMP;
    port* a = ports[1];
    port* b = ports[2];
    port* gt = ports[0];
    comp->width = max(a->connection->width, b->connection->width);
    comp->sign = a->connection->sign | b->connection->sign;
    a->width = comp->width;
    a->sign = comp->sign;
    b->width = comp->width;
    b->sign = comp->sign;
    gt->width = 1;
    gt->sign = 0;
    comp->inputs["a"] = a;
    comp->inputs["b"] = b;
    comp->outputs["gt"] = gt;
}

void ComponentParser::parse_lt(vector<port*> ports)
{
    component* comp = data_manager->components.back();
    comp->type = ComponentType::COMP;
    port* a = ports[1];
    port* b = ports[2];
    port* lt = ports[0];
    comp->width = max(a->connection->width, b->connection->width);
    comp->sign = a->connection->sign | b->connection->sign;
    a->width = comp->width;
    a->sign = comp->sign;
    b->width = comp->width;
    b->sign = comp->sign;
    lt->width = 1;
    lt->sign = 0;
    comp->inputs["a"] = a;
    comp->inputs["b"] = b;
    comp->outputs["lt"] = lt;
}

void ComponentParser::parse_eq(vector<port*> ports)
{
    component* comp = data_manager->components.back();
    comp->type = ComponentType::COMP;
    port* a = ports[1];
    port* b = ports[2];
    port* eq = ports[0];
    comp->width = max(a->connection->width, b->connection->width);
    comp->sign = a->connection->sign | b->connection->sign;
    a->width = comp->width;
    a->sign = comp->sign;
    b->width = comp->width;
    b->sign = comp->sign;
    eq->width = 1;
    eq->sign = 0;
    comp->inputs["a"] = a;
    comp->inputs["b"] = b;
    comp->outputs["eq"] = eq;
}

void ComponentParser::parse_mux2x1(vector<port*> ports)
{
    component* mux2x1 = data_manager->components.back();
    mux2x1->type = ComponentType::MUX2x1;
    port* sel = ports[1];
    port* b = ports[2];
    port* a = ports[3];
    port* d = ports[0];
    mux2x1->width = d->connection->width;
    mux2x1->sign = d->connection->sign;
    sel->width = 1;
    sel->sign = 0;
    b->width = mux2x1->width;
    b->sign = mux2x1->sign;
    a->width = mux2x1->width;
    a->sign = mux2x1->sign;
    d->width = mux2x1->width;
    d->sign = mux2x1->sign;
    mux2x1->inputs["sel"] = sel;
    mux2x1->inputs["b"] = b;
    mux2x1->inputs["a"] = a;
    mux2x1->outputs["d"] = d;
}
void ComponentParser::parse_shr(vector<port*> ports)
{
    component* shr = data_manager->components.back();
    shr->type = ComponentType::SHR;
    port* a = ports[1];
    port* sh_amt = ports[2];
    port* d = ports[0];
    shr->width = d->connection->width;
    shr->sign = d->connection->sign;
    a->width = shr->width;
    a->sign = shr->sign;
    sh_amt->width = shr->width;
    sh_amt->sign = 0;
    d->width = shr->width;
    d->sign = shr->sign;
    shr->inputs["a"] = a;
    shr->inputs["sh_amt"] = sh_amt;
    shr->outputs["d"] = d;
}
void ComponentParser::parse_shl(vector<port*> ports)
{
    component* shl = data_manager->components.back();
    shl->type = ComponentType::SHL;
    port* a = ports[1];
    port* sh_amt = ports[2];
    port* d = ports[0];
    shl->width = d->connection->width;
    shl->sign = d->connection->sign;
    a->width = shl->width;
    a->sign = shl->sign;
    sh_amt->width = shl->width;
    sh_amt->sign = 0;
    d->width = shl->width;
    d->sign = shl->sign;
    shl->inputs["a"] = a;
    shl->inputs["sh_amt"] = sh_amt;
    shl->outputs["d"] = d;
}
void ComponentParser::parse_div(vector<port*> ports)
{
    component* div = data_manager->components.back();
    div->type = ComponentType::DIV;
    port* a = ports[1];
    port* b = ports[2];
    port* qout = ports[0];
    div->width = qout->connection->width;
    div->sign = qout->connection->sign;
    a->width = div->width;
    a->sign = div->sign;
    b->width = div->width;
    b->sign = b->sign;
    qout->width = div->width;
    qout->sign = div->sign;
    div->inputs["a"] = a;
    div->inputs["b"] = b;
    div->outputs["qout"] = qout;
}
void ComponentParser::parse_mod(vector<port*> ports)
{
    component* mod = data_manager->components.back();
    mod->type = ComponentType::MOD;
    port* a = ports[1];
    port* b = ports[2];
    port* rem = ports[0];
    mod->width = rem->connection->width;
    mod->sign = rem->connection->sign;
    a->width = mod->width;
    a->sign = mod->sign;
    b->width = mod->width;
    b->sign = mod->sign;
    rem->width = mod->width;
    rem->sign = mod->sign;
    mod->inputs["a"] = a;
    mod->inputs["b"] = b;
    mod->outputs["rem"] = rem;
}
void ComponentParser::parse_inc(vector<port*> ports)
{
    component* inc = data_manager->components.back();
    inc->type = ComponentType::INC;
    port* a = ports[1];
    port* d = ports[0];
    inc->width = d->connection->width;
    inc->sign = d->connection->sign;
    a->width = inc->width;
    a->sign = inc->sign;
    d->width = inc->width;
    d->sign = inc->sign;
    inc->inputs["a"] = a;
    inc->outputs["d"] = d;
}
void ComponentParser::parse_dec(vector<port*> ports)
{
    component* dec = data_manager->components.back();
    dec->type = ComponentType::DEC;
    port* a = ports[1];
    port* d = ports[0];
    dec->width = d->connection->width;
    dec->sign = d->connection->sign;
    a->width = dec->width;
    a->sign = dec->sign;
    d->width = dec->width;
    d->sign = dec->sign;
    dec->inputs["a"] = a;
    dec->outputs["d"] = d;
}

} // namespace Parser