CXX = g++
WARNINGS = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable
CXXFLAGS = -Wall -g

all: server client
# Executable dependencies
server: server.o board.o
	$(CXX) $(CXXFLAGS) -o server server.o board.o

client: client.o
	$(CXX) $(CXXFLAGS) -o client client.o board.o

board: board.o

# Object Files
server.o: server.cpp board.h
	$(CXX) $(CXXFLAGS) -c server.cpp

client.o: client.cpp board.h
	$(CXX) $(CXXFLAGS) -c client.cpp

board.o: board.h

clean:
	rm -rf server \
		client \
		board 