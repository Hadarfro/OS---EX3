CXX = clang++
CXXFLAGS = -std=c++11 -Wall
BOOST_LIBS = -lboost_system -lpthread

SERVER_SRC = ex1.cpp
SERVER_BIN = ex1

CLIENT_SRC = ex1_client.cpp

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN) $(BOOST_LIBS)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

.PHONY: all clean
