#include "FileWriter.h"
#include "DataManager.h"
#include <regex>

namespace Writer
{
    FileWriter::FileWriter(DataManager* data_manager)
        : data_manager(data_manager)
        {}
    void FileWriter::determineModuleName(string filePath)
    {
        int startIdx = filePath.find_last_of("/\\") + 1;
        int endIdx = filePath.find_last_of(".");
        if (endIdx < 0)
        {
            endIdx = filePath.length();
        }
        string fileName = filePath.substr(startIdx, endIdx-startIdx);
        if ((fileName.substr(0,5) == "474a_") || (fileName.substr(0,5) == "574a_"))
        {
            moduleName = fileName.substr(5);
        }
        else
        {
            moduleName = fileName;
        }
    }
    void FileWriter::declareTimescale()
    {
        circuitFile << "`timescale 1ns/1ns" << endl << endl;
    }
    void FileWriter::declareModule()
    {
        circuitFile << "module " << moduleName;
        circuitFile << "(";
        bool firstInput = true;
        for (wire*& currWire : data_manager->wires)
        {
            if ((currWire->type == WireType::INPUT) || (currWire->type == WireType::OUTPUT))
            {
                if (!firstInput)
                {
                    circuitFile << ", ";
                }
                firstInput = false;
                circuitFile << currWire->name;
            }
        }
        circuitFile << ");" << endl << endl;
    }
    void FileWriter::terminateModule()
    {
        circuitFile << "endmodule" << endl;
    }
    void FileWriter::declareNet(wire* net)
    {
        if (net->type == WireType::INPUT)
        {
            circuitFile << "    input ";
        }
        else if (net->type == WireType::OUTPUT)
        {
            circuitFile << "    output ";
        }
        else
        {
            circuitFile << "    wire ";
        }
        if (net->sign)
        {
            circuitFile << "signed ";
        }
        if (net->width > 1)
        {
            circuitFile << "[" << net->width - 1 << ":0] ";
        }
        circuitFile << net->name << ";" << endl;
    }
    void FileWriter::declareInputs()
    {
        for (wire*& currWire : data_manager->wires)
        {
            if (currWire->type == WireType::INPUT)
            {
                declareNet(currWire);
            }
        }
        circuitFile << endl;
    }
    void FileWriter::declareOutputs()
    {
        for (wire*& currWire : data_manager->wires)
        {
            if (currWire->type == WireType::OUTPUT)
            {
                declareNet(currWire);
            }
        }
        circuitFile << endl;
    }
    void FileWriter::declareWires()
    {
        for (wire*& currWire : data_manager->wires)
        {
            if ((currWire->type == WireType::WIRE) || (currWire->type == WireType::REGISTER))
            {
                declareNet(currWire);
            }
        }
        circuitFile << endl;
    }
    void FileWriter::declareCasts()
    {
        bool atLeastOneCast = false;
        for (component*& currComponent : data_manager->components)
        {
            if (currComponent->type == ComponentType::CAST)
            {
                atLeastOneCast = true;
                wire* output = currComponent->outputs["out"]->connection;
                wire* input = currComponent->inputs["in"]->connection;
                circuitFile << "    assign " << output->name << " = ";
                if (output->sign != input->sign)
                {
                    if (output->sign)
                    {
                        circuitFile << "$signed(";
                    }
                    else
                    {
                        circuitFile << "$unsigned(";
                    }
                }
                if (output->width != input->width)
                {
                    if (output->width > input->width)
                    {
                        int numBitsToAdd = output->width - input->width;
                        if (input->sign)
                        {
                            circuitFile << "{{" << numBitsToAdd << "{" << input->name << "[" << input->width - 1 << "]}}, " << input->name << "}";
                        }
                        else
                        {
                            circuitFile << "{{" << numBitsToAdd << "{1'b0}}, " << input->name << "}";
                        }
                    }
                    else
                    {
                        circuitFile << input->name << "[" << output->width-1 << ":0]";
                    }
                }
                else
                {
                    circuitFile << input->name;
                }
                if (output->sign != input->sign)
                {
                    circuitFile << ");" << endl;
                }
                else
                {
                    circuitFile << ";" << endl;
                }
            }
        }
        if (atLeastOneCast)
        {
            circuitFile << endl;
        }
    }
    void FileWriter::declareComponents()
    {
        for (component*& currComponent : data_manager->components)
        {
            if (currComponent->type != ComponentType::CAST)
            {
                string componentType = ComponentTypeToStr(currComponent->type);
                if (currComponent->sign)
                {
                    componentType = "S" + componentType;
                }
                circuitFile << "    " << componentType;
                circuitFile << " #(.DATAWIDTH(" << currComponent->width << ")) ";
                circuitFile << currComponent->name << "(";
                auto begin = currComponent->inputs.begin();
                auto end = currComponent->inputs.end();
                bool firstWire = true;
                for (auto it = begin; it != end; ++it)
                {
                    if (!firstWire)
                    {
                        circuitFile << ", ";
                    }
                    firstWire = false;
                    circuitFile << "." << it->first << "(" << it->second->connection->name << ")";
                }
                begin = currComponent->outputs.begin();
                end = currComponent->outputs.end();
                for (auto it = begin; it != end; ++it)
                {
                    if (!firstWire)
                    {
                        circuitFile << ", ";
                    }
                    firstWire = false;
                    circuitFile << "." << it->first << "(" << it->second->connection->name << ")";
                }
                circuitFile << ");" << endl;
            }
        }
        circuitFile << endl;
    }
    void FileWriter::run(string filePath)
    {
        openFile (filePath);
        determineModuleName(filePath);
        declareTimescale();
        declareModule();
        declareInputs();
        declareOutputs();
        declareWires();
        declareCasts();
        declareComponents();
        terminateModule();
        closeFile ();
    }
    void FileWriter::openFile(string filePath)
    {
        circuitFile.open (filePath);
    }
    void FileWriter::closeFile()
    {
        circuitFile.close ();
    }
}
