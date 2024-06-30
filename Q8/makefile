# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror

# Source files
SOURCES = main.cpp Reactor.cpp Proactor.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = main

# Target to create the executable
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Target to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to clean the build files
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets
.PHONY: all clean
