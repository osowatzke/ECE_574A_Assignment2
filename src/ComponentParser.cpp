#include "ComponentParser.h"

#include "DataTypes.h"

#include <iostream>
#include <vector>

using namespace DataPathGen;
using namespace std;

namespace Parser
{
    
ComponentParser::ComponentParser(DataManager* data_manager)
    : data_manager(data_manager) {}

int ComponentParser::parse_line(string line)
{
    smatch match;
    const regex component_regex{"^\\s*(\\w+)\\s*=\\s*(\\w+)\\s*(\\S+)?\\s*(\\w+)?\\s*(\\S+)?\\s*(\\w+)?\\s*$"};
    regex_match(line, match, component_regex);
    if (!match.empty())
    {
        vector <string> wire_names;
        vector <string> operators;
        wire_names.push_back(match.str(1));
        for (size_t i = 2; i < match.size(); ++ i)
        {
            if (match.str(i) != "")
            {
                if ((i % 2) == 0)
                {
                    wire_names.push_back(match.str(i));
                }
                else
                {
                    operators.push_back(match.str(i));
                }
            }
        }
        component* new_component = new component;
        vector <port*> ports;
        bool firstWire = true;
        for (string& wire_name : wire_names)
        {
            if (wire_name != "1")
            {  
                wire* wire_to_connect = data_manager->find_wire(wire_name);
                if (wire_to_connect == NULL)
                {
                    wire_to_connect = new wire;
                    wire_to_connect->name = wire_name;
                    wire_to_connect->width = 1;
                    wire_to_connect->sign = 0;
                    wire_to_connect->src = NULL;
                    data_manager->wires.push_back(wire_to_connect);
                    undefined_wires.push_back(wire_to_connect);
                    cout << "Adding wire " << wire_name << endl;
                }
                port* new_port = new port;
                if (firstWire)
                {
                    wire_to_connect->src = new_port;
                }
                else
                {
                    wire_to_connect->dest.push_back(new_port);
                }
                firstWire = false;
                new_port->connection = wire_to_connect;
                new_port->parent = new_component;
                ports.push_back(new_port);
            }
        }
        data_manager->components.push_back(new_component);
        if (operators.size() == 0)
        {
            parse_reg(ports);
        }
        else if (operators.size() == 1)
        {
            if (operators[0] == "+")
            {
                if (ports.size() == 2)
                {
                    parse_inc(ports);
                }
                else
                {
                    parse_add(ports);
                }
            }
            else if (operators[0] == "-")
            {
                if (ports.size() == 2)
                {
                    parse_dec(ports);
                }
                else
                {
                    parse_sub(ports);
                }
            }
            else if (operators[0] == "*")
            {
                parse_mul(ports);
            }
            else if (operators[0] == ">")
            {
                parse_gt(ports);
            }
            else if (operators[0] == "<")
            {
                parse_lt(ports);
            }
            else if (operators[0] == "==")
            {
                parse_eq(ports);
            }
            else if (operators[0] == ">>")
            {
                parse_shr(ports);
            }
            else if (operators[0] == "<<")
            {
                parse_shl(ports);
            }
            else if (operators[0] == "/")
            {
                parse_div(ports);
            }
            else if (operators[0] == "%")
            {
                parse_mod(ports);
            }
            else
            {
                cout << "ERROR: Incorrect operator '" << operators[0] << "'" << endl;
                return 1;
            }
        }
        else
        {
            if (operators[0] == "?" && operators[1] == ":")
            {
                parse_mux2x1(ports);
            }
            else
            {
                cout << "ERROR: Incorrect Operator '" << operators[0] << "' and '" << operators[1] << "'" << endl;
                return 1;
            }
        }
    }
    return 0;
}
int ComponentParser::parse_lines(vector<string> lines)
{
    int retVal;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        retVal = parse_line(lines[i]);
        if (retVal)
        {
            return retVal;
        }
    }
    for (wire*& undefined_wire : undefined_wires)
    {
        if (undefined_wire->src == NULL)
        {
            cout << "ERROR: Missing Input " << undefined_wire->name << endl;
            return 1;
        }
        else if (undefined_wire->dest.size() == 0)
        {
            cout << "ERROR: Missing Output " << undefined_wire->name << endl;
            return 1;
        }
        else
        {
            cout << "ERROR: Missing Wire " << undefined_wire->name << endl;
            return 1;
        }
    }
    return 0;
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