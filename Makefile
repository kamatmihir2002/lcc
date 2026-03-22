CC = gcc
CFLAGS= -g -Iinclude

SRCS := $(wildcard src/*.c)

OBJS := $(patsubst %.c,%.o,$(SRCS))

TARGET = bin/lcc


.PHONY: all clean

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.INTERMEDIATE: $(OBJS)

all: $(TARGET)

clean: binclean
	rm $(OBJS)

binclean:
	rm $(TARGET)
	
	
