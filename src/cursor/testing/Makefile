CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
ASANFLAGS = -fsanitize=address
LDFLAGS = $(ASANFLAGS)


EXEC = test-cursor

# src files
SRCS = test-cursor.c ../cursor.c ../../tui/tui.c 

DEPS = ../cursor.h ../../tui/tui.h

# obj files
OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) -c -o  $@ $< $(CFLAGS) $(ASANFLAGS)

all: $(EXEC)


$(EXEC): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)



clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
