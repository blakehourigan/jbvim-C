#ifndef GAP_BUFFER_H_INCLUDED
#define GAP_BUFFER_H_INCLUDED

// each line and doc buffer should have 150 elements to begin
const int INIT_SIZE = 150;

typedef struct DocumentGapBuffer {
    GapBuffer *start;  // pointer to the first element
    GapBuffer *end;    // pointer to last element
    GapBuffer *buffer; // pointer to the actual buffer of GapBuffer elements

    int filled_items;
} GapBufferDoc;

int str_len(char *str);

DocumentGapBuffer *init(char *str);

void insert_into_doc(GapBufferDoc *doc, GapBuffer *item);

void move_gap_right_doc(GapBufferDoc *doc);

void move_gap_left_doc(GapBufferDoc *doc);

void write_content(GapBufferDoc *doc, int start, int end);

void grow_buffer_doc(GapBufferDoc *buffer);

void delete_item_doc(GapBufferDoc *doc);

#endif
