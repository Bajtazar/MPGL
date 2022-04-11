CXX	:= g++11.1
CXX_FLAGS	:= -std=c++20 -O3 -ggdb3 -lGL -lglfw -ldl -pthread

BIN	:= bin
SRC	:= $(shell find src/ -name "*.cpp")
T_SRC	:= $(shell find . -name "*.cpp" | awk '{ if (/main/ && !seen) { seen = 1 } else print }')
INCLUDE	:= ./include

EXECUTABLE	:= Epsilon
TEST_EXEC	:= Test

all: test app

run: clean all
	clear
	./$(BIN)/$(TEST_EXEC)
	./$(BIN)/$(EXECUTABLE)

app: $(SRC) glad.c
	clear
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $(BIN)/$(EXECUTABLE)

test: $(T_SRC) glad.c
	clear
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $(BIN)/$(TEST_EXEC)

clean:
	-rm $(BIN)/*
