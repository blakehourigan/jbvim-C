#ifndef DOC_BUFFER_H_INCLUDED
#define DOC_BUFFER_H_INCLUDED

#include "line_buffer.h"

// each line and doc buffer should have 150 elements to begin
const int INIT_DOC_SIZE = 150;

typedef struct DocumentGapBuffer {
    lineBuffer *start; // a pointer to the START of the buffer.
    lineBuffer *end; // a pointer to the END of the buffer. NOT end of content,
                     // end of BUFFER.
    lineBuffer *buffer; // pointer to the buffer object itself.

    int filled_items; // keeps track of how many items are actually inside of
                      // the buffer.
    int size;
} DocumentGapBuffer;

DocumentGapBuffer init_document(char *str);

void insert_into_doc(DocumentGapBuffer *document, lineBuffer item);

void move_gap_right_doc(DocumentGapBuffer *doc);

void move_gap_left_doc(DocumentGapBuffer *doc);

void write_content(DocumentGapBuffer *doc, int start, int end);

void grow_buffer_doc(DocumentGapBuffer *buffer);

void delete_item_doc(DocumentGapBuffer *doc);

#endif
