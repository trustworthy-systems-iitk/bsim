#Introduction

<tt>bsim</tt> is an algorithmic inference tool that processes gate-level netlists. The
input to the tool is a flat verilog netlist consisting of gates and latches.
The output of the tool is an _abstracted_ verilog netlist that contains 
inferred high-level components such adders, subtractors, register files etc.

##Compiling and Running <tt>bsim</tt>

Make sure your system has the necessary libraries installed. These are:

* IBM CPLEX Solver
* GLPK Solver
* Boost C++ libraries v1.50 or higher

You can compile without CPLEX if you set USECPLEX=0 in verilog/makefile.  Note
this makes the overlap resolution step much slower because GLPK is a lot slower
than CPLEX. Also note the quality of results will be a little worse because the
GPLEX formulation does not implemente what the journal paper calls the
"sliceable" formulation (section IVB).
   
Step 1: Clone the <tt>bsim repository.</tt>

    $ hg clone https://spramod@bitbucket.org/spramod/bsim-tetc14

Step 2: Build CUDD
     
    $  cd bsim/cudd-2.4.2/
    $  make
    $  cd obj/
    $  make
    $  cd ../..
     
Step 3: Build MiniSAT

    $  cd minisat/
    $  export MROOT=`pwd`
    $  cd core/
    $  make libr

Step 4: Build <tt>bsim</tt>

    $ cd verilog/
    $ make

If all this succeeds, you have successfully built <tt>bsim.</tt>

##Running <tt>bsim</tt>

One example run is as follows:

    $ ./bsim -o configs/default.xml tests/router_flat.v

This command asks <tt>bsim to analyze the design in tests/router_flat.v. The </tt>
output files produced by BSIM using this command are:

* results/router_flat.summary : this contains a summary of the inferred
  components found by <tt>bsim.  </tt>
* results/verilog/router_flat.v and results/verilog/router_flat.library.v: 
  these two files contain an "abstracted" version of the router design.


#References

This software is published in two papers:

1. "Reverse Engineering Digital Circuits Using Structural and Functional Analyses", 
Pramod Subramanyan, Nestan Tsiskaridze, Wenchao Li, Adria Gascon, Wei Yang Tan, Anish Tiwari, Nishanth Shankar, Sanjit Seshia and Sharad Malik 
IEEE Transactions on Emerging Topics in Computing, March 2014.

2. ''Reverse engineering digital circuits using functional analysis", 
Pramod Subramanyan, Nestan Tsiskaridze, Kanika Pasricha, Dillon Reisman, Adriana Susnea, Sharad Malik 
Proceedings of the Conference on Design, Automation and Test in Europe (DATE), 2013.

If you use this software in a publication of your own, it is suggested that you cite the journal paper.
