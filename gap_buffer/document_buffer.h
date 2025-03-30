#ifndef DOC_BUFFER_H_INCLUDED
#define DOC_BUFFER_H_INCLUDED

#include "line_buffer.h"

// each line and doc buffer should have 150 elements to begin
const int INIT_DOC_SIZE = 150;

typedef struct DocumentGapBuffer {
    lineBuffer *start;  // pointer to the first element
    lineBuffer *end;    // pointer to last element
    lineBuffer *buffer; // pointer to the actual buffer of GapBuffer elements

    int filled_items;
} DocumentGapBuffer;

DocumentGapBuffer *init(char *str);

void insert_into_doc(DocumentGapBuffer *document, lineBuffer *item);

void move_gap_right_doc(DocumentGapBuffer *doc);

void move_gap_left_doc(DocumentGapBuffer *doc);

void write_content(DocumentGapBuffer *doc, int start, int end);

void grow_buffer_doc(DocumentGapBuffer *buffer);

void delete_item_doc(DocumentGapBuffer *doc);

#endif
