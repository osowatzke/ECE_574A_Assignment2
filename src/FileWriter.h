#ifndef GUARD_FileWriter_h
#define GUARD_FileWriter_h

#include "DataManager.h"
#include <fstream>
#include <iostream>

using namespace DataPathGen;

namespace Writer
{
    class FileWriter
    {
        public:
            FileWriter(DataManager* data_manager);
            void run(string filePath);
        private:
            DataManager* data_manager;
            ofstream circuitFile;
            string moduleName;
            void openFile(string filePath);
            void closeFile();
            void determineModuleName(string filePath);
            void declareModule();
            void terminateModule();
            void declareNet(wire* net);
            void declareInputs();
            void declareOutputs();
            void declareWires();
            void declareCasts();
    };
}

#endif //GUARD_FileWriter_h