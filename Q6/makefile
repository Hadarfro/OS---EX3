CXX = clang++
CXXFLAGS = -std=c++11 -Werror -Wsign-conversion -g
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

SOURCES = main.cpp Reactor.cpp algo6.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLES = main algo6

.PHONY: all clean valgrind

all: $(EXECUTABLES)

main: main.o Reactor.o
	$(CXX) $(CXXFLAGS) -o $@ $^

algo6: algo6.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

valgrind: $(EXECUTABLES)
	valgrind $(VALGRIND_FLAGS) ./algo3-server
	valgrind $(VALGRIND_FLAGS) ./algo3

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)
