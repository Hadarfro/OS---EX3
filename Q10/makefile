CXX = clang++
CXXFLAGS = -std=c++11 -Werror -Wsign-conversion -g
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

SOURCES = main.cpp Proactor.cpp algo10.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLES = main algo10

.PHONY: all clean valgrind

all: $(EXECUTABLES)

main: main.o Proactor.o
	$(CXX) $(CXXFLAGS) -o $@ $^

algo10: algo10.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

valgrind: $(EXECUTABLES)
	valgrind $(VALGRIND_FLAGS) ./algo3-server
	valgrind $(VALGRIND_FLAGS) ./algo3

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)