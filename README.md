#Introduction

bsim is an algorithmic inference tool that processes gate-level netlists. The
input to the tool is a flat verilog netlist consisting of gates and latches.
The output of the tool is an _abstracted_ verilog netlist that contains 
inferred high-level components such adders, subtracters, register files etc.

##Compiling bsim

###Step 0: Requirements.

bsim was developed on Linux. Some people have had success installing and running
it on Macs, but this requires hacking the makefile. These instructions assume
you are running a relatively recent version of Linux.

Make sure your system has the necessary libraries installed. These are:

* IBM CPLEX Solver
* GLPK Solver
* Boost C++ libraries v1.50 or higher

You can compile without CPLEX if you set USECPLEX=0 in verilog/makefile.  Note
this makes the overlap resolution step much slower because GLPK is a lot slower
than CPLEX. Also note the quality of results will be a little worse because the
GPLEX formulation does not implement what the journal paper calls the
"sliceable" formulation (section IVB).
   
###Step 1: Clone the bsim repository.

    $ hg clone https://spramod@bitbucket.org/spramod/bsim-tetc14

###Step 2: Build CUDD
     
    $  cd bsim/cudd-2.4.2/
    $  make
    $  cd obj/
    $  make
    $  cd ../..
     
###Step 3: Build MiniSAT

    $  cd minisat/
    $  export MROOT=`pwd`
    $  cd core/
    $  make libr

###Step 4: Build bsim

    $ cd verilog/
    $ make

If all this succeeds, you have successfully built bsim.

##Running bsim

One example run is as follows:

    $ ./bsim -o configs/default.xml tests/router_flat.v

This command asks bsim to analyze the design in tests/router_flat.v. The 
output files produced by bsim using this command are:

* results/router_flat.summary : this contains a summary of the inferred
  components found by bsim.  
* results/verilog/router_flat.v and results/verilog/router_flat.library.v: 
  these two files contain an "abstracted" version of the router design.

bsim has a lot of options, all of which are specified using the config XML file
that controls its operation. You may want to look through the bsim_options_t
structure in main.h to see what these are.  One example is the "dumpWords"
option which can be used to dump aggregated words (groups of bits are operated
open together) that were discovered by bsim.

#References

This software is published in two papers:

1. "Reverse Engineering Digital Circuits Using Structural and Functional Analyses", 
Pramod Subramanyan, Nestan Tsiskaridze, Wenchao Li, Adria Gascon, Wei Yang Tan, Anish Tiwari, Nishanth Shankar, Sanjit Seshia and Sharad Malik 
IEEE Transactions on Emerging Topics in Computing, March 2014.

2. ''Reverse engineering digital circuits using functional analysis", 
Pramod Subramanyan, Nestan Tsiskaridze, Kanika Pasricha, Dillon Reisman, Adriana Susnea, Sharad Malik 
Proceedings of the Conference on Design, Automation and Test in Europe (DATE), 2013.

If you use this software in a publication of your own, it is suggested that you cite the journal paper.
