### Configuring Visual Studio Code

Follow the steps in this article to Configure VS Code for Microsoft C++:

[https://code.visualstudio.com/docs/cpp/config-msvc](https://code.visualstudio.com/docs/cpp/config-msvc)

Follow the steps in this article to install MINGW:

[https://code.visualstudio.com/docs/cpp/config-mingw](https://code.visualstudio.com/docs/cpp/config-mingw)

Follow the steps in this article to install CMAKE:

[https://code.visualstudio.com/docs/cpp/cmake-linux](https://code.visualstudio.com/docs/cpp/cmake-linux)

Note that you may have to download directly from the CMAKE site instead of the link in the Visual Studio article:

[https://cmake.org/download/](https://cmake.org/download/)

### Compiling the Code

If compiling for the first time, make a build directory by running `mkdir build` from the VS Code terminal.

Then, navigate to the build directory, by running `cd build`.

Once you are in the build directory, type ctrl+shift+p to open the Command Pallete. Once the Command Pallete is open, select "CMake: Configure".

Then from the build directory, run the following command to configure the cmake project `cmake ..`

Finally, run `cmake --build .` to compile.

Once the build directory is created, you should only have to run the cmake commands from the build directory.

### Running the Code

The Datapath Generator can be run from the build directory by issuing the following command:

`.\src\dpgen.exe ..\circuits\<circuit_name> <output_verilog_file>`

where <circuit_name> is the name of the circuit you wish to parse and the <output_verilog_file> is the path to the output verilog file.

### Debugging the Code

Refer to launch.json in the .vscode directory. It is currently configured to debug "474a_circuit1.txt". However, the launch.json file can be modified to support debugging of any data path generator run.

To debug, click on the Run and Debug tab and hit the play button to start a "Debug Data Path Generator" run.

### Reference Critical Paths

| Circuit           | Critical Path |
| ----------------- | ------------- |
| 474a_circuit1.txt | 15.688 ns     |
| 474a_circuit2.txt | 39.511 ns     |
| 474a_circuit3.txt | 48.267 ns     |
| 474a_circuit4.txt | 35.514 ns     |
| 574a_circuit5.txt | 267.765 ns    |
| 574a_circuit6.txt | 267.765 ns    |
| mixedcircuit1.txt | 4.924 ns      |
| mixedcircuit2.txt | 7.270 ns      |
| mixedcircuit3.txt | 4.924 ns      |
| ucircuit1.txt     | 4.924 ns      |
| ucircuit2.txt     | 7.270 ns      |
| ucircuit3.txt     | 4.924 ns      |

### Autogenerating Verilog Files

Autogenerate the required verilog files by running

`..\scripts\generate_verilog_files.cmd`

from the build directory.

**Note: you should rerun this command every time you pull from main**

### Validating the Autogenerated Verilog Files

Open vivado and navigate to the repo root directory. Next, run the following command from the tcl console:

`source ./scripts/project.tcl`

Run each of the testbenches and validate that none of the signals have high impedance ('Z') or unknown ('X') values. Also confirm that the error signal is not asserted.

Once the vivado project is generated, you don't need to rerun the "project.tcl" script. Instead, you can simply update the project sources by running

`source ./scripts/update_sources.tcl`

from the tcl console.
