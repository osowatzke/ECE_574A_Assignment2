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
        circuitFile << ")" << endl;
    }
    void FileWriter::terminateModule()
    {
        circuitFile << "endmodule" << endl;
    }
    void FileWriter::run(string filePath)
    {
        openFile (filePath);
        determineModuleName(filePath);
        declareModule();
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
