CXX	:= /usr/local/gcc-11.1.0/bin/g++11.1
CXX_FLAGS	:= -std=c++20 -ggdb -lGL -lglfw  -ldl -lm -lpthread -pthread

BIN	:= bin
SRC	:= src
INCLUDE	:= include

LIBRARIES	:=
EXECUTABLE	:= Epsilon



all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp glad.c
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
