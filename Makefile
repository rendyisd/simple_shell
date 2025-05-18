CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = sh.c utils.c tokenizer.c
OBJS_PATH = $(addprefix build/,$(SRCS:.c=.o))
TARGET = build/sh

all: $(TARGET)

build/:
	mkdir -p build/

$(TARGET): $(OBJS_PATH)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c | build/
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/

.PHONY: all clean
