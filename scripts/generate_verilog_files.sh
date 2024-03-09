#!/usr/bin/bash

cd ../circuits
circuit_files=$(ls)
cd ../build
if [ -d ../verilog_files/autogen ]; then
    rm -rf ../verilog_files/autogen
fi
mkdir ../verilog_files/autogen
for circuit_file in $circuit_files
do
    verilog_file=${circuit_file#474a_}
    verilog_file=${verilog_file#574a_}
    verilog_file=${verilog_file/.txt/_autogen.v}
    echo "src/dpgen ../circuits/$circuit_file ../verilog_files/autogen/$verilog_file"
    src/dpgen ../circuits/$circuit_file ../verilog_files/autogen/$verilog_file
done
