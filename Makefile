CC = gcc

CFLAGS = -g -Wall -O0 -std=c99

SOURCES = ./src/nes6502.c ./src/nes6502_test.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = nes6502

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core