CC = gcc
CFLAGS= -O1 -Iinclude

SRCS := $(wildcard src/*.c)

OBJS := $(patsubst %.c,%.o,$(SRCS))

TARGET = bin/ucc


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
	
	
