PROFILE:=0
DEBUG?=0
FEZ:=0
USECPLEX?=1

CC=gcc
CXX:=g++
LD=g++

ifeq ($(PROFILE), 1)
    PGFLAGS:=-pg
else
    PGFLAGS:=
endif

ifeq ($(DEBUG), 1)
    DBGFLAGS:=-g
    OPTFLAGS:=
else
    DBGFLAGS:=
    OPTFLAGS:=-O6
endif


# CUDD vars
CUDDROOT=../cudd-2.4.2/
CUDDINCLUDE=-I${CUDDROOT}/include/
TTMATHINCLUDE=-I../ttmath/

CUDDLIBS=$(CUDDROOT)cudd/libcudd.a $(CUDDROOT)util/libutil.a  $(CUDDROOT)obj/libobj.a $(CUDDROOT)/epd/libepd.a $(CUDDROOT)mtr/libmtr.a $(CUDDROOT)st/libst.a $(CUDDROOT)dddmp/libdddmp.a
CUDDLIBRARIES=-L$(CUDDROOT)cudd/ -L$(CUDDROOT)util/  -L$(CUDDROOT)obj/ -L$(CUDDROOT)/epd/ -L$(CUDDROOT)mtr/ -L$(CUDDROOT)st/ -L$(CUDDROOT)dddmp/
CUDDLIBFLAGS=-lobj -ldddmp -lcudd -lmtr -lst -lutil -lepd

ifeq ($(USECPLEX), 1)
    CPLEXDEFINE=-DUSE_CPLEX
    CPLEXINCLUDE=-I/opt/ibm/ILOG/CPLEX_Studio125/cplex/include/ -I/opt/ibm/ILOG/CPLEX_Studio125/concert/include/
    CPLEXLIBRARIES=-L/opt/ibm/ILOG/CPLEX_Studio125/concert/lib/x86-64_sles10_4.1/static_pic/ -L/opt/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic/
    CPLEXLIBFLAGS=-lilocplex -lcplex -lconcert -lm -lpthread
else
    CPLEXDEFINE=-DUSE_GLPK
    CPLEXINCLUDE=
    CPLEXLIBRARIES=
    CPLEXLIBFLAGS=
endif

# minisat vars
MINISATROOT=../minisat/
MINISATLIB=${MINISATROOT}/core/lib_release.a
MINISATINCLUDE=-I${MINISATROOT}

INCLUDE=${CUDDINCLUDE} ${MINISATINCLUDE} ${TTMATHINCLUDE} ${CPLEXINCLUDE}
LIBS=-lfl -lz -lglpk

DEFINES=-D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS -D IL_STD ${CPLEXDEFINE} # -DMODULE_CONSTRAINTS : this last flag is for backward compatibility with the "buggy" ILP formulation.

CXXFLAGS:=-c -Wall -Werror ${INCLUDE} ${DEFINES} ${DBGFLAGS} ${OPTFLAGS} ${PGFLAGS} -Wno-unused-result -Wno-unused-function
LDFLAGS:=${DBGFLAGS} ${OPTFLAGS} ${PGFLAGS} $(CUDDLIBRARIES)  ${CPLEXLIBRARIES}
SUFFIXES += .d

#Find all the C++ files in the current directory.
SOURCES:=$(shell find . -name "*.cpp")
#These are the dependency files, which make will clean up after it creates them
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))
#Objects
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))

all:bsim simre simplify_test bmtest

lex.yy.o: lex.yy.cc
	$(CXX) $(CXXFLAGS) lex.yy.cc

lex.yy.cc: structv.l structv.tab.cc
	flex -o lex.yy.cc structv.l

structv.tab.o: structv.tab.cc ast.h
	$(CXX) $(CXXFLAGS) structv.tab.cc

structv.tab.cc: structv.y ast.h
	bison -d -Wall -Werror -rall -o structv.tab.cc structv.y

clean:
	rm -f bsim simre simplify_test *.o *.cc *.d

# These rules are for creating the final binary.

bsim: lex.yy.o structv.tab.o ${CUDDLIBS} ${MINISATLIB} $(OBJECTS)
	$(LD) $(LDFLAGS) -o bsim $(OBJECTS) lex.yy.o structv.tab.o ${LIBS} ${CUDDLIBFLAGS} ${MINISATLIB} ${CPLEXLIBFLAGS}

simplify_test: bsim
	rm -f ./simplify_test
	ln -s ./bsim ./simplify_test

simre: bsim
	rm -f ./simre
	ln -s ./bsim ./simre

bmtest: bsim
	rm -f ./bmtest
	ln -s ./bsim ./bmtest

# This is the rule for creating the dependency files
%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< > $@

# This include statement pulls in the commands to build the .o files.
include $(subst .cpp,.d,$(SOURCES))
