@echo off
SetLocal EnableDelayedExpansion
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