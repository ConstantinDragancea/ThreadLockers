CC=gcc
FLAGS=-O0 -g
SRC:=$(wildcard ./src/*.c)
DEMOS:=$(wildcard ./demos/*.c)
LIB:=-Isrc -pthread
SANITIZER:=-fsanitize=undefined,signed-integer-overflow
WARNINGS:= -Wall -Wextra

COMP_FLAGS:=$(FLAGS) $(SANITIZER) $(WARNINGS)

all:
	$(CC) $(COMP_FLAGS) $(LIB) $(SRC) ./main.c -o main.out

mutex_demo:
	$(CC) $(COMP_FLAGS) $(LIB) $(SRC) ./demos/mutex_demo.c -o mutex_demo.out

semaphore_demo:
	$(CC) $(COMP_FLAGS) $(LIB) $(SRC) ./demos/semaphore_demo.c -o semaphore_demo.out

demos: mutex_demo semaphore_demo

clean:
	rm *.out
	rm *.a
	rm *.o