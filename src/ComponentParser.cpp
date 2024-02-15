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
    const ParserTableEntry ADD_ENTRY = {.r=ADD_REGEX,.func=parse_add};
    const ParserTableEntry SUB_ENTRY = {.r=SUB_REGEX,.func=parse_sub};
    PARSER_TABLE = {ADD_ENTRY, SUB_ENTRY};
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
    reg->inputs["d"] = data_manager->find_wire(match.str(2));
    reg->outputs["q"] = data_manager->find_wire(match.str(1));
}
void ComponentParser::parse_add(smatch match)
{
    component* add = data_manager->components.back();
    add->type = ComponentType::ADD;
    add->inputs["a"] = data_manager->find_wire(match.str(2));
    add->inputs["b"] = data_manager->find_wire(match.str(3));
    add->outputs["sum"] = data_manager->find_wire(match.str(1));
}
void ComponentParser::parse_sub(smatch match)
{
    component* sub = data_manager->components.back();    
    sub->type = ComponentType::SUB;
    sub->inputs["a"] = data_manager->find_wire(match.str(2));
    sub->inputs["b"] = data_manager->find_wire(match.str(3));
    sub->outputs["diff"] = data_manager->find_wire(match.str(1));
}
void ComponentParser::parse_mul(smatch match)
{
    component* mul = data_manager->components.back();
    mul->type = ComponentType::MUL;
    mul->inputs["a"] = data_manager->find_wire(match.str(2));
    mul->inputs["b"] = data_manager->find_wire(match.str(3));
    mul->outputs["prod"] = data_manager->find_wire(match.str(1));
}
void ComponentParser::parse_comp(smatch match)
{
    component* comp = data_manager->components.back();
    comp->type = ComponentType::COMP;
    comp->inputs["a"] = data_manager->find_wire(match.str(2));
    comp->inputs["b"] = data_manager->find_wire(match.str(4));
    comp->outputs["gt"] = NULL;
    comp->outputs["lt"] = NULL;
    comp->outputs["eq"] = NULL;
    if (match.str(3) == ">")
    {
        comp->outputs["gt"] = data_manager->find_wire(match.str(1));
    }
    else if (match.str(3) == "<")
    {
        comp->outputs["lt"] = data_manager->find_wire(match.str(1));
    }
    else
    {
        comp->outputs["eq"] = data_manager->find_wire(match.str(1));
    }
}
void ComponentParser::parse_mux2x1(smatch match)
{
    component* mux2x1 = data_manager->components.back();
    mux2x1->type = ComponentType::MUX2x1;
    mux2x1->inputs["sel"] = data_manager->find_wire(match.str(2));
    mux2x1->inputs["b"] = data_manager->find_wire(match.str(3));
    mux2x1->inputs["a"] = data_manager->find_wire(match.str(4));
    mux2x1->outputs["d"] = data_manager->find_wire(match.str(1));
}
void ComponentParser::parse_shr(smatch match)
{
    component* shr = data_manager->components.back();
    shr->type = ComponentType::SHR;
    shr->inputs["a"] = data_manager->find_wire(match.str(2));
    shr->inputs["sh_amt"] = data_manager->find_wire(match.str(3));
    shr->outputs["d"] = data_manager->find_wire(match.str(1));
}
void ComponentParser::parse_shl(smatch match)
{
    component* shl = data_manager->components.back();
    shl->type = ComponentType::SHL;
    shl->inputs["a"] = data_manager->find_wire(match.str(2));
    shl->inputs["sh_amt"] = data_manager->find_wire(match.str(3));
    shl->outputs["d"] = data_manager->find_wire(match.str(1));
}
} // namespace Parser