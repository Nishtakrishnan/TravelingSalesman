# Executable names:
EXE = final_project
TEST = test

# Add all object files needed for compiling:
EXE_OBJ = main.o
OBJS = Graph.o Dsets.o Kruskal.o Eulerian_Path.o Christofides.o main.o

# Use the cs225 makefile template:
include cs225/make/cs225.mk