### Configuring Visual Studio Code

Follow the steps in this article to Configure VS Code for Microsoft C++:

[https://code.visualstudio.com/docs/cpp/config-msvc](https://code.visualstudio.com/docs/cpp/config-msvc)

Follow the steps in this article to install MINGW:

[https://code.visualstudio.com/docs/cpp/config-mingw](https://code.visualstudio.com/docs/cpp/config-mingw)

Follow the steps in this article to install CMAKE:

[https://code.visualstudio.com/docs/cpp/cmake-linux](https://code.visualstudio.com/docs/cpp/cmake-linux)

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

### Debugging the code

Refer to launch.json in the .vscode directory. It is currently configured to debug "474a_circuit1.txt". However, the launch.json file can be modified to support debugging of any parsing run.

To debug, click on the Run and Debug tab and hit the play button to start a "Debug File Parser" run.
