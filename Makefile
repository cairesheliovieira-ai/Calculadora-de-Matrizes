CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g
LIBS = -lm
TARGET = calculadora
OBJ = main.o matrix.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(TARGET)

main.o: main.c matrix.h
	$(CC) $(CFLAGS) -c main.c -o main.o

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o

clean:
	rm -f $(OBJ) $(TARGET)
