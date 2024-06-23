CXX = clang++
CXXFLAGS = -std=c++11 -Werror -Wsign-conversion -g
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

SOURCES = algo3-server.cpp algo3.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLES = algo3-server algo3

.PHONY: all clean valgrind

all: $(EXECUTABLES)

algo3-server: algo3-server.o
	$(CXX) $(CXXFLAGS) -o $@ $^

algo3: algo3.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

valgrind: $(EXECUTABLES)
	valgrind $(VALGRIND_FLAGS) ./algo3-server
	valgrind $(VALGRIND_FLAGS) ./algo3

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)
