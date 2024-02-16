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
    ParserTableEntry COMP_ENTRY;
    COMP_ENTRY.r = COMP_REGEX;
    COMP_ENTRY.func = &ComponentParser::parse_comp;
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
            COMP_ENTRY, MUX2x1_ENTRY, SHR_ENTRY, SHL_ENTRY,
            DIV_ENTRY, MOD_ENTRY, INC_ENTRY, DEC_ENTRY};
}
void ComponentParser::parse_line(string line)
{
    for (size_t i = 0; i < PARSER_TABLE.size(); ++i)
    {
        smatch match;
        regex_match(line, match, PARSER_TABLE[i].r);
        if (!match.empty())
        {
            data_manager->components.push_back(new component);
            (*this.*(PARSER_TABLE[i].func))(match);
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
void ComponentParser::parse_reg(smatch match)
{
    component* reg = data_manager->components.back();
    reg->type = ComponentType::REG;
    wire* d = data_manager->find_wire(match.str(2));
    wire* q = data_manager->find_wire(match.str(1));
    d->dest.push_back(reg);
    q->src.push_back(reg);
    reg->inputs["d"] = d;
    reg->outputs["q"] = q;
}
void ComponentParser::parse_add(smatch match)
{
    component* add = data_manager->components.back();
    add->type = ComponentType::ADD;
    wire* a = data_manager->find_wire(match.str(2));
    wire* b = data_manager->find_wire(match.str(3));
    wire* sum = data_manager->find_wire(match.str(1));
    a->dest.push_back(add);
    b->dest.push_back(add);
    sum->src.push_back(add);
    add->inputs["a"] = a;
    add->inputs["b"] = b;
    add->outputs["sum"] = sum;
}
void ComponentParser::parse_sub(smatch match)
{
    component* sub = data_manager->components.back();    
    sub->type = ComponentType::SUB;
    wire* a = data_manager->find_wire(match.str(2));
    wire* b = data_manager->find_wire(match.str(3));
    wire* diff = data_manager->find_wire(match.str(1));
    a->dest.push_back(sub);
    b->dest.push_back(sub);
    diff->src.push_back(sub);
    sub->inputs["a"] = a;
    sub->inputs["b"] = b;
    sub->outputs["diff"] = diff;
}
void ComponentParser::parse_mul(smatch match)
{
    component* mul = data_manager->components.back();
    mul->type = ComponentType::MUL;
    wire* a = data_manager->find_wire(match.str(2));
    wire* b = data_manager->find_wire(match.str(3));
    wire* prod = data_manager->find_wire(match.str(1));
    a->dest.push_back(mul);
    b->dest.push_back(mul);
    prod->src.push_back(mul);
    mul->inputs["a"] = a;
    mul->inputs["b"] = b;
    mul->outputs["prod"] = prod;
}
void ComponentParser::parse_comp(smatch match)
{
    component* comp = data_manager->components.back();
    comp->type = ComponentType::COMP;
    wire* a = data_manager->find_wire(match.str(2));
    wire* b = data_manager->find_wire(match.str(4));
    wire* c = data_manager->find_wire(match.str(1));
    a->dest.push_back(comp);
    b->dest.push_back(comp);
    c->src.push_back(comp);
    comp->inputs["a"] = a;
    comp->inputs["b"] = b;
    comp->outputs["gt"] = NULL;
    comp->outputs["lt"] = NULL;
    comp->outputs["eq"] = NULL;
    if (match.str(3) == ">")
    {
        comp->outputs["gt"] = c;
    }
    else if (match.str(3) == "<")
    {
        comp->outputs["lt"] = c;
    }
    else
    {
        comp->outputs["eq"] = c;
    }
}
void ComponentParser::parse_mux2x1(smatch match)
{
    component* mux2x1 = data_manager->components.back();
    mux2x1->type = ComponentType::MUX2x1;
    wire* sel = data_manager->find_wire(match.str(2));
    wire* b = data_manager->find_wire(match.str(3));
    wire* a = data_manager->find_wire(match.str(4));
    wire* d = data_manager->find_wire(match.str(1));
    a->dest.push_back(mux2x1);
    b->dest.push_back(mux2x1);
    sel->dest.push_back(mux2x1);
    d->src.push_back(mux2x1);
    mux2x1->inputs["sel"] = data_manager->find_wire(match.str(2));
    mux2x1->inputs["b"] = data_manager->find_wire(match.str(3));
    mux2x1->inputs["a"] = data_manager->find_wire(match.str(4));
    mux2x1->outputs["d"] = data_manager->find_wire(match.str(1));
}
void ComponentParser::parse_shr(smatch match)
{
    component* shr = data_manager->components.back();
    shr->type = ComponentType::SHR;
    wire* a = data_manager->find_wire(match.str(2));
    wire* sh_amt = data_manager->find_wire(match.str(3));
    wire* d = data_manager->find_wire(match.str(1));
    a->dest.push_back(shr);
    sh_amt->dest.push_back(shr);
    d->src.push_back(shr);
    shr->inputs["a"] = a;
    shr->inputs["sh_amt"] = sh_amt;
    shr->outputs["d"] = d;
}
void ComponentParser::parse_shl(smatch match)
{
    component* shl = data_manager->components.back();
    shl->type = ComponentType::SHL;
    wire* a = data_manager->find_wire(match.str(2));
    wire* sh_amt = data_manager->find_wire(match.str(3));
    wire* d = data_manager->find_wire(match.str(1));
    a->dest.push_back(shl);
    sh_amt->dest.push_back(shl);
    d->src.push_back(shl);
    shl->inputs["a"] = a;
    shl->inputs["sh_amt"] = sh_amt;
    shl->outputs["d"] = d;
}
void ComponentParser::parse_div(smatch match)
{
    component* div = data_manager->components.back();
    div->type = ComponentType::DIV;
    wire* a = data_manager->find_wire(match.str(2));
    wire* b = data_manager->find_wire(match.str(3));
    wire* qout = data_manager->find_wire(match.str(1));
    a->dest.push_back(div);
    b->dest.push_back(div);
    qout->src.push_back(div);
    div->inputs["a"] = a;
    div->inputs["b"] = b;
    div->outputs["qout"] = qout;
}
void ComponentParser::parse_mod(smatch match)
{
    component* mod = data_manager->components.back();
    mod->type = ComponentType::MOD;
    wire* a = data_manager->find_wire(match.str(2));
    wire* b = data_manager->find_wire(match.str(3));
    wire* rem = data_manager->find_wire(match.str(1));
    a->dest.push_back(mod);
    b->dest.push_back(mod);
    rem->src.push_back(mod);
    mod->inputs["a"] = a;
    mod->inputs["b"] = b;
    mod->outputs["rem"] = rem;
}
void ComponentParser::parse_inc(smatch match)
{
    component* inc = data_manager->components.back();
    inc->type = ComponentType::INC;
    wire* a = data_manager->find_wire(match.str(2));
    wire* d = data_manager->find_wire(match.str(1));
    a->dest.push_back(inc);
    d->dest.push_back(inc);
    inc->inputs["a"] = a;
    inc->outputs["d"] = d;
}
void ComponentParser::parse_dec(smatch match)
{
    component* dec = data_manager->components.back();
    dec->type = ComponentType::DEC;
    wire* a = data_manager->find_wire(match.str(2));
    wire* d = data_manager->find_wire(match.str(1));
    a->dest.push_back(dec);
    d->dest.push_back(dec);
    dec->inputs["a"] = data_manager->find_wire(match.str(2));
    dec->outputs["d"] = data_manager->find_wire(match.str(1));
}
} // namespace Parser