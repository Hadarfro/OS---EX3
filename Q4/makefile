CXX = clang++
CXXFLAGS = -std=c++11 -Werror -Wsign-conversion -g
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

SOURCES = algo4-server.cpp algo4.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLES = algo4-server algo4

.PHONY: all clean valgrind

all: $(EXECUTABLES)

algo4-server: algo4-server.o
	$(CXX) $(CXXFLAGS) -o $@ $^

algo4: algo4.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

valgrind: $(EXECUTABLES)
	valgrind $(VALGRIND_FLAGS) ./algo3-server
	valgrind $(VALGRIND_FLAGS) ./algo3

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)
