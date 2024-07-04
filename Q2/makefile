CXX = clang++
CXXFLAGS = -std=c++11 -Werror -Wsign-conversion -g
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

SOURCES = algo2-2.cpp TestCounter.cpp Test.cpp
OBJECTS = $(SOURCES:.cpp=.o)

run: algo2-2 test
	

algo2-2: algo2-2.o $(filter-out TestCounter.o Test.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) -v $^ -o algo2-2

test: TestCounter.o Test.o $(filter-out algo2-2.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) -v $^ -o test

tidy:
	clang-tidy $(SOURCES) -checks=bugprone-,clang-analyzer-,cppcoreguidelines-,performance-,portability-,readability-,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

valgrind: algo2-2 test
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./algo2-2 2>&1 | { egrep "lost| at " || true; }
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./test 2>&1 | { egrep "lost| at " || true; }

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o algo2-2 test