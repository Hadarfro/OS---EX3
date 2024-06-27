CXX = clang++
CXXFLAGS = -std=c++11 -Werror -Wsign-conversion -g
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

SOURCES = algo7-server.cpp algo7.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLES = algo7-server algo7

.PHONY: all clean valgrind

all: $(EXECUTABLES)

algo7-server: algo7-server.o
	$(CXX) $(CXXFLAGS) -o $@ $^

algo7: algo7.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

valgrind: $(EXECUTABLES)
	valgrind $(VALGRIND_FLAGS) ./algo3-server
	valgrind $(VALGRIND_FLAGS) ./algo3

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)
