1) Names and NetIDs of Group Members:
   Owen Sowatzke           - osowatzke
   Matthew Nolan           - mdnolan
   Kazi Safkat             - safkat
   Muhtasim Alam Chowdhury - mmc7

2) Course Enrolled:
   Owen Sowatzke           - ECE574A
   Matthew Nolan           - ECE474A
   Kazi Safkat             - ECE574A
   Muhtasim Alam Chowdhury - ECE574A

3) Brief Description of the Program:
   In this assignment, we created a High Level Synthesis (HLS) tool that converts behavioral netlists into synthesizable
   verilog datapath implementations. Additionally, our pragram returns the critical path of the datapath by utilizing
   latency estimates of the datapath components. The arguments for executing the program are the path to the behavioral
   netlist and the path to the output file.

   The program creates a graph topography representing the control data flow graph of the netlist. This graph is generated
   in two stages. First, all components explicitly written in the file are added to the graph in the file parser. Then,
   the components implicitly in the graph are added. These include registers without component instantiations and
   cast components which convert between data types. The cast components have zero delay and map directly to assign
   statements in the output verilog file. The clock and reset are also implicitly added in this stage of the program.
   
   The edges of the graph are instances of wire structures, which correspond to inputs, outputs, wires, or registers
   in the netlist. The nodes of the graph are instances of component structures, which correspond to different
   components in the netlist.
   
   The graph is then converted into a verilog file by mapping the array of wires into inputs, outputs, and wires in the
   verilog file. The array of components is then mapped into assign statments and instantations of modules.
   
   The graph is also used to compute the critical path. The critical path calculation is performed by recursively
   traversing the graph starting with all inputs and registers. The maximum delay from each starting point (input or
   register) to ending point (output or register) is computed during this recursive search. The maximum delay is the
   critical path of the circuit.

4) Individual Contributions of Group Members:
   All group members collaborated in implementing, testing and debugging the functionality of the program. Below are the 
   individual contributions of each member:
    - Owen Sowatzke
        - Contributed to the data manager and program structure
        - Contributed to the file parser
        - Generated testbenches for mixed circuits
        - Create helper scripts to accelerate testing and development

    - Matthew Nolan
        - Contributed to the data manager and program structure
        - Contributed to the implicit component generator

    - Kazi Safkat
        - Contributed to the file writer
        - Implemented signed datapath components and unsigned circuits
        - Generated testbenches for signed and unsigned circuits

    - Muhtasim Alam Chowdhury
        - Contributed to the critical path functionality
        - Implemented signed datapath components and signed circuits