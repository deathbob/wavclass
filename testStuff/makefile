#
#   Makefile - Steven J Zeil, Old Dominion University
#              June 1996
#
#  This is a "self-building" makefile for use in any project where the
#  gcc/g++ compilers are to be used to build a single program.
#
#  (It can be modified to handle building multiple programs. See the notes
#  at the end of the file for this.).
#
#  What does it do for you?  It keeps track of the #include dependencies
#  as your program evolves.  If you add or delete an #include in your
#  source code, you don't need to alter this makefile.  It will
#  note the change and automatically produce your program with the minimal
#  amount of re-compilation.
#
#
#  Copy this file into your project directory.
#  Your directory should contain all the .c, .C, .cc, or .cpp files needed
#  for your program.
#
#  Edit your copy of this Makefile, supplying the appropriate values for
#  the CFLAGS, CPPFLAGS, TARGET, SOURCES, LINK, and LFLAGS variables.
#
#  The command
#     make
#  will actually compile and build your program.
#  (The first time you use this Makefile in any directory, you may get
#  a warning about a missing "make.dep" file.  Ignore this message. This 
#  missing file will be generated for you.)
#
#  The command
#     make clean
#  will "clean up" your directory by deleting the executable program and
#  the various by-products of the compilation process.  Your source code
#  will be left unharmed.
#
#
########################################################################
# Macro definitions for "standard" C and C++ compilations
#
# Edit the next 5 definitions. After that, "make" will
#   build the program.
# 
#  Define special compilation flags for C++ compilation. These may change when
#  we're done testing and debugging.
CPPFLAGS=-g
#
#  Define special compilation flags for C compilation. These may change when
#  we're done testing and debugging.
CFLAGS=-g
# 
#  What is the name of the program you want to create?  (See below for notes
#     on using this makefile to generate multiple programs.)
TARGET=test.exe
#
#  List the object code files to be produced by compilation. Normally this 
#  list will include one ".o" file for each C++ file (with names ending in 
#  ".cpp", ".cc" or ".C"), and/or each C file (with names ending in ".c").
#  Do NOT list .h files. For example, if you are building a program from 
#  source files foo.c, foo.h, bar.cpp, baz.cc, and bam.h you would use
#      OBJS1=foo.o bar.o baz.o
OBJS=test.o
# 
#  What program should be used to link this program? If the program is
#  even partly C++, use g++.  If it is entirely C, use gcc.
LINK=g++ $(CPPFLAGS)
#LINK=gcc $(CFLAGS)
# 
#  Define special linkage flags.  Usually, these are used to include
#  special libraries of code, e.g., -lm to add the library of mathematical
#  routines such as sqrt, sin, cos, etc.
LFLAGS=-lm
#
#
#
#  In most cases, you should not change anything below this line.
#
#  The following is "boilerplate" to set up the standard compilation
#  commands:
#
.SUFFIXES:
.SUFFIXES: .d .o .h .c .cc .C .cpp
.c.o: ; $(CC) $(CFLAGS) -MMD -c $*.c
.cc.o: ; $(CPP) $(CPPFLAGS) -MMD -c $*.cc 
.C.o: ; $(CPP) $(CPPFLAGS) -MMD -c $*.C
.cpp.o: ; $(CPP) $(CPPFLAGS) -MMD -c $*.cpp

CC=gcc
CPP=g++

%.d: %.c
	touch $@
%.d: %.cc
	touch $@
%.d: %.C
	touch $@
%.d: %.cpp
	touch $@

DEPENDENCIES = $(OBJS:.o=.d)

# 
# Targets:
# 
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK) $(FLAGS) -o $(TARGET) $(OBJS) $(LFLAGS)

clean:
	-rm -f $(TARGET) $(OBJS) $(DEPENDENCIES) make.dep


make.dep: $(DEPENDENCIES)
	-cat $(DEPENDENCIES) > make.dep

include make.dep


########################################################################
#
#  Advanced notes:  managing multiple programs with one makefile
#
#    If you want to create 2 or more programs (usually in the same directory)
#    using a single makefile, do the following:
#
#      1) For each program that you want to build, make distinct copies of
#         the TARGET= and OBJS= lines, adding a number to distinguish each.
#         For example:
#              TARGET1=myprogram1
#              TARGET2=myprogram2
#              OBJS1=mymain1.o foo.o bar.o
#              OBJS2=mymain2.o foo.o baz.o
#      2) Redefine TARGET and OBJS as the combination of the TARGETi and 
#         OBJSi lists.  For example,
#              TARGET=$(TARGET1) $(TARGET2)
#              OBJS=$(OBJS1) $(OBJS2)
#      3) In the "Dependencies:" area near the end of the file, find
#         the lines
#           $(TARGET): $(OBJS)
#       	$(LINK) $(FLAGS) -o $(TARGET) $(OBJS) $(LFLAGS)
#         and, as in step 1, make distinct, numbered copies for each program
#         you want to build.  For example,
#           $(TARGET1): $(OBJS1)
#       	$(LINK) $(FLAGS) -o $(TARGET1) $(OBJS1) $(LFLAGS)
#           $(TARGET2): $(OBJS2)
#       	$(LINK) $(FLAGS) -o $(TARGET2) $(OBJS2) $(LFLAGS)
#         These copies replace the original 2 lines.
#
########################################################################
#  Changes: 
#     6/4/96  Fixes to allow use under both "standard" Sun "make" and under
#             the GNU make.
#             Added notes on building multiple targets.
