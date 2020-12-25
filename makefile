CC=gcc
FLAGS=-O0 -g
SRC:=$(wildcard ./src/*.c)
LIB:=-Isrc
SANITIZER:=-fsanitize=address,undefined,signed-integer-overflow
WARNINGS:= -Wall -Wextra

COMP_FLAGS:=$(FLAGS) $(SANITIZER) $(WARNINGS)

all:
	$(CC) $(COMP_FLAGS) $(LIB) $(SRC) -o main.out

clean:
	rm *.out
	rm *.a
	rm *.o