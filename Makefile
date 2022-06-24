CXX	:= g++11.1
CXX_FLAGS	:= -std=c++20 -O3 -ggdb3 -lGL -lglfw -ldl -pthread

BIN	:= bin
SRC	:= $(shell find src/ -name "*.cpp")
T_SRC	:= $(shell find src/ -name "*.cpp") $(shell find tests/ -name "*.cpp")
INCLUDE	:= ./include
SNK_SRC := $(shell find examples/Snake/ -name "*.cpp")
SNK_INC := ./examples/Snake/include

EXECUTABLE	:= Epsilon
SNAKE_EXEC	:= Snake
TEST_EXEC	:= Test

all: test app

run: clean all
	clear
	./$(BIN)/$(TEST_EXEC)
	./$(BIN)/$(EXECUTABLE)

app: $(SRC) glad.c main.cpp
	clear
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $(BIN)/$(EXECUTABLE)

test: $(T_SRC) glad.c
	clear
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $(BIN)/$(TEST_EXEC)

snake: $(SRC) $(SNK_SRC) glad.c
	clear
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(SNK_INC) $^ -o $(BIN)/$(SNAKE_EXEC)

clean:
	-rm $(BIN)/*
