CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = sh.c
SRCS_PATH = $(addprefix src/,$(SRCS))
OBJS_PATH = $(addprefix build/,$(SRCS:.c=.o))
TARGET = build/sh

all: $(TARGET)

build/:
	mkdir -p build/

$(TARGET): $(OBJS_PATH)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS_PATH): $(SRCS_PATH) | build/
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/

.PHONY: all clean
