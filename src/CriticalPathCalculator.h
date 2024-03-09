#ifndef GUARD_CriticalPathCalculator
#define GUARD_CriticalPathCalculator

#include "DataManager.h"
#include "DataTypes.h"

using namespace DataPathGen;

namespace DataPathGen
{

    const double COMPONENT_DELAY[NUM_COMPONENT_TYPES][6] =
        //1-bit  2-bit  8-bit   16-bit  32-bit  64-bit
        {{2.616, 2.644,  2.879,  3.061,  3.602,   3.966},  // REG
         {2.704, 3.713,  4.924,  5.638,  7.270,   9.566},  // ADD
         {3.024, 3.412,  4.890,  5.569,  7.253,   9.566},  // SUB
         {2.438, 3.651,  7.453,  7.811, 12.395,  15.354},  // MUL
         {3.031, 3.934,  5.949,  6.256,  7.264,   8.416},  // COMP
         {4.083, 4.115,  4.815,  5.623,  8.079,   8.766},  // MUX2x1
         {3.644, 4.007,  5.178,  6.460,  8.819,  11.095},  // SHR
         {3.614, 3.980,  5.152,  6.549,  8.565,  11.220},  // SHL
         {0.619, 2.144, 15.439, 33.093, 86.312, 243.233},  // DIV
         {0.758, 2.149, 16.078, 35.563, 88.142, 250.583},  // MOD
         {1.792, 2.218,  3.111,  3.471,  4.347,   6.200},  // INC
         {1.792, 2.218,  3.108,  3.701,  4.685,   6.503},  // DEC
         {0.000, 0.000,  0.000,  0.000,  0.000,   0.000}}; // CAST

    class CriticalPathCalculator
    {
        public:
            CriticalPathCalculator(DataManager* data_manager);
            void run();
        private:
            DataManager* data_manager;
            void outputCriticalPath();
            double computeCriticalPath();
            double computeCriticalPath(wire* currWire);
            double getComponentDelay(component* currComponent);
    };
} // namespace DataPathGen

#endif // GUARD_CriticalPathCalculator