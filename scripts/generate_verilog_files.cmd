@echo off
SetLocal EnableDelayedExpansion
:: set circuit_dir=..\circuits
:: set cwd=%cd%
cd ..\circuits
set circuit_files=
for /f %%i in ('dir /b') do (set circuit_files=!circuit_files!;%%i)
cd ..\build
rmdir ..\verilog_files\autogen /s /q
mkdir ..\verilog_files\autogen
for %%i in (%circuit_files%) do (
    set verilog_file=%%i
    set verilog_file=!verilog_file:.txt=_autogen.v!
    set verilog_file=!verilog_file:474a_=!
    set verilog_file=!verilog_file:574a_=!
    echo ".\src\dpgen.exe ..\circuits\%%i ..\verilog_files\autogen\!verilog_file!"
    .\src\dpgen.exe ..\circuits\%%i ..\verilog_files\autogen\!verilog_file!
)

:: set out_dir=../verilog_files/autogen
:: set netlist_files=474_circuit1.txt 474a_circuit2.txt 474a_circuit3.txt 474a_circuit4.txt ^
:: 574a_circuit5.txt 574a_circuit6.txt mixedcircuit1.txt mixedcircuit2.txt mixedcircuit3.txt  ^
:: ucircuit1.txt ucircuit2.txt ucircuit3.txt
:: set autogen_files=circuit1.v circuit2.v circuit3.v circuit4.v^
:: circuit5.v circuit6.v mixedcircuit1.v mixedcircuit2.v mixedcircuit3.v  ^
:: ucircuit1.v ucircuit2.v ucircuit3.v
:: for %%i in (%netlist_files%) do (
::     .\src\dpgen ..\circuits\%%i ..\verilog_files\autogen\temp.v"
::     set /a j+=1
::     echo %j%
:: )