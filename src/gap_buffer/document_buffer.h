#ifndef DOC_BUFFER_H_INCLUDED
#define DOC_BUFFER_H_INCLUDED

#include "line_buffer.h"

// each line and doc buffer should have 150 elements to begin
#define INIT_DOC_SIZE 150

typedef struct DocumentGapBuffer {
    lineBuffer *start; // a pointer to the START of the buffer.
    lineBuffer *end; // a pointer to the END of the buffer. NOT end of content,
                     // end of BUFFER.
    lineBuffer *buffer; // pointer to the buffer object itself.

    int used_lines; // keeps track of how many lines are in **use** by the user
                    // currently.
    int size;       // total current allocated size

    char *file_name_ptr; // ptr to the file name variable. makes freeing memory
                         // possible.
} DocumentGapBuffer;

DocumentGapBuffer init_document(char *str);

void insert_line_doc(DocumentGapBuffer *document, char *content);

void move_gap_right_doc(DocumentGapBuffer *document);

void move_gap_left_doc(DocumentGapBuffer *document);

void move_to_document_start(DocumentGapBuffer *document);
void move_to_document_end(DocumentGapBuffer *document);

void write_content(DocumentGapBuffer *doc, int start, int end);

void print_content(DocumentGapBuffer *document);

lineBuffer *grab_line(DocumentGapBuffer *document, int line_number);

lineBuffer *grab_previous_line(DocumentGapBuffer *document, lineBuffer *line);

lineBuffer *grab_next_line(DocumentGapBuffer *document, lineBuffer *line);

void grow_document(DocumentGapBuffer *document);

void dbg_buffer_doc(DocumentGapBuffer *document);

void dbg_doc_simple(DocumentGapBuffer *document);

void delete_document_line(DocumentGapBuffer *document);

void cleanup_document(DocumentGapBuffer *document);

#endif
