CC = gcc 
CFLAGS = -Wall -Wextra -g -std=c11
ASANFLAGS = -fsanitize=address
LDFLAGS = $(ASANFLAGS)


EXEC = main

# src files 
SRCS = document_buffer.c line_buffer.c


# obj files
OBJS = $(SRCS:.c=.o)

all: $(EXEC)


$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c document_buffer.h line_buffer.h 
	$(CC) $(CFLAGS) $(ASANFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
