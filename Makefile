#########################################################
#              Makefile for Graphs Project              #
#########################################################
# Author: Sam Hecht
# Date: 12/18/22
# Purpose: A Makefile to compile all the files that the MetroSim
#			needs to create an executable
 

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3

# This rule builds the graphs executable by linking the .o files
graphs: graphs.o priorityQueue.o
	$(CXX) $(LDFLAGS) $^ -o graphs

# This rule builds graphs.o
graphs.o: graphs.cpp priorityQueue.h node.h
	$(CXX) $(CXXFLAGS) -c graphs.cpp

# This rule builds priorityQueue.o
priorityQueue.o: priorityQueue.cpp priorityQueue.h node.h
	$(CXX) $(CXXFLAGS) -c priorityQueue.cpp


# remove executables, object code, and temporary files
# from the current folder
# -- the executable created by unit_test is called a.out
clean: 
	rm *.o *~ a.out
