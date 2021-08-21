CXX	:= g++11.1
CXX_FLAGS	:= -std=c++20 -O3 -ggdb3 -lGL -lglfw -ldl -pthread -ltbb

BIN	:= bin
SRC	:= $(shell find . -name "*.cpp")
INCLUDE	:= include

LIBRARIES	:=
EXECUTABLE	:= Epsilon



all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC) glad.c
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
