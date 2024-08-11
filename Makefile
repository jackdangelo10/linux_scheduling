CC = gcc
CFLAGS = -std=c17 -Wall -g
SRCS = $(wildcard *.c)
EXES = $(SRCS:.c=)

all: $(EXES)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(EXES)