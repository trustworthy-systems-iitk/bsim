#! /bin/bash

cd cudd-2.4.2 && make && cd obj && make && cd ../.. && cd minisat && export MROOT=$PWD && cd core && make libr && cd ../.. && cd verilog && make -j4
