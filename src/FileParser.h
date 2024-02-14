#ifndef GUARD_FileParser_h
#define GUARD_FileParser_h
#include <string>
#include <vector>

using namespace std;

namespace FileParsing
{
    struct signal {
        string name;
        int width;
        bool sign;
    };

    class FileParser{
        public:
            vector<string> lines;
            vector<signal> inputs;
            vector<signal> outputs;
            vector<signal> wires;
            vector<signal> registers;
            int read_lines(string file_path);
            void read_signals(vector<signal> & signals, string signal_type);
            void read_inputs();
            void read_outputs();
            void read_wires();
            void read_registers();
    };

    // enum SignalType {Input, Output, Wire, Register};


}

#endif //GUARD_FileParser_h