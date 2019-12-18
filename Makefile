CC		:= gcc
C_FLAGS := -Wall -Wextra

BIN		:= bin
SRC		:= src
INCLUDE		:= include
LIB		:= lib

LIBRARIES	:= -I/usr/local/include -L/usr/local/lib -lgsl -lgslcblas -lm

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
else
EXECUTABLE	:= main
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	-$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)
