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
    void FileWriter::declareModule()
    {
        circuitFile << "module " << moduleName;
        circuitFile << "(";
        bool firstInput = true;
        for (wire*& currWire : data_manager->wires)
        {
            if (currWire->type == WireType::INPUT)
            {
                if (!firstInput)
                {
                    circuitFile << ", ";
                }
                firstInput = false;
                circuitFile << currWire->name;
            }
        }
        circuitFile << ")" << endl << endl;
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
    void FileWriter::run(string filePath)
    {
        openFile (filePath);
        determineModuleName(filePath);
        declareModule();
        declareInputs();
        declareOutputs();
        declareWires();
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
