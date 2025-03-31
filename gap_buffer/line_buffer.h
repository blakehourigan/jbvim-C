#ifndef LINE_BUFFER_H_INCLUDED
#define LINE_BUFFER_H_INCLUDED

// each line and should have 150 elements to begin
#define INIT_LINE_SIZE 150

typedef struct lineBuffer {
    char *start; // a pointer to the START of the buffer.
    char *end; // a pointer to the END of the buffer. NOT end of content, end of
               // BUFFER.
    char *buffer; // pointer to the buffer object itself.

    int filled_items; // keeps track of how many items are actually inside of
                      // the buffer.
    int size;
} lineBuffer;

lineBuffer init_line(char *line_content);

void move_gap_right_line(lineBuffer *line);

void move_gap_left_line(lineBuffer *line);

void insert_left_line(lineBuffer *line, char character);

void grow_line(lineBuffer *line);

void delete_item_line(lineBuffer *line);

void move_to_line_start(lineBuffer *line);

int move_to_line_end(lineBuffer *line);

#endif
