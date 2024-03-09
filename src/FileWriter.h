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
            int run(string filePath);
        private:
            DataManager* data_manager;
            ofstream circuitFile;
            string moduleName;
            int openFile(string filePath);
            void closeFile();
            void determineModuleName(string filePath);
            void declareTimescale();
            void declareModule();
            void terminateModule();
            void declareNet(wire* net);
            void declareInputs();
            void declareOutputs();
            void declareWires();
            void declareCasts();
            void declareComponents();
    };
}

#endif //GUARD_FileWriter_h