### Configuring Visual Studio Code

Follow the steps in this article to Configure VS Code for Microsoft C++:

[https://code.visualstudio.com/docs/cpp/config-msvc](https://code.visualstudio.com/docs/cpp/config-msvc)

### Compiling the code

If compiling for the first time, make a build directory by running `mkdir build` from the VS Code terminal.

Then, navigate to the build directory, by running `cd build`.

From the build directory, run the following command to configure the cmake project `cmake ..`

Finally, run `cmake --build .` to compile.

Once the build directory is created, you should only have to run the cmake commands from the build directory.

### Running the code

There is currently a `FileParser` which supports parsing behavioral netlists. It can be run from the build directory by issuing the following command:

`.\src\FileParser.exe ..\circuits\<circuit_name>`

where <circuit_name> is the name of the circuit you wish to parse.
