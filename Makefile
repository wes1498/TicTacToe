
CXX = g++
WARNINGS = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable
CXXFLAGS = -Wall -g

# Executable dependencies
main: main.o board.o
	$(CXX) $(CXXFLAGS) -o main main.o board.o
board: Board.o

# Object Files
main.o: main.cpp board.h
	$(CXX) $(CXXFLAGS) -c main.cpp
Board.o: board.h