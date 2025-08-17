CC = gcc 
CFLAGS = -Wall -Wextra -g -std=c11
ASANFLAGS = -fsanitize=address
LDFLAGS = $(ASANFLAGS)


EXEC = jbvim 

# src files 
SRCS = main.c cursor/cursor.c tui/tui.c gap_buffer/line_buffer.c gap_buffer/document_buffer.c

DEPS = gap_buffer/document_buffer.h gap_buffer/line_buffer.h 

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
