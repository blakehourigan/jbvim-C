CC = gcc 
CFLAGS = -Wall -Wextra -g -std=c11
ASANFLAGS = -fsanitize=address
LDFLAGS = $(ASANFLAGS)

# src files 
SRCS = document_buffer.c line_buffer.c

# obj files
OBJS = $(SRCS:.c=.o)

EXEC = main

%.o: %.c document_buffer.h line_buffer.h 
	$(CC) $(CFLAGS) $(ASANFLAGS) -c $< -o $@

all: $(EXEC)
# 	^^^^^^^^ this is the dependency list

#  By putting the object files in the dependency list and in the rule, make knows it must first compile the .c versions individually, 
#  and then build the executable hellomake. 
$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)



clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
