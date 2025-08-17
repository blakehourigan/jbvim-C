CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
ASANFLAGS = -fsanitize=address
LDFLAGS = $(ASANFLAGS)


PROJECT_HOME = /home/blake/Documents/Github/jbvim-C/src

EXEC = testing-gap-buffer

# src files
SRCS = testing-gap-buffer.c $(PROJECT_HOME)/gap_buffer/document_buffer.c $(PROJECT_HOME)/gap_buffer/line_buffer.c 

DEPS = $(PROJECT_HOME)/gap_buffer/document_buffer.c  $(PROJECT_HOME)/gap_buffer/line_buffer.c 

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
