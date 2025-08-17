#include "document_buffer.h"
#include "line_buffer.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

DocumentGapBuffer init_document(char *file_name) {
    DocumentGapBuffer document;

    // enough room for 150 (initially) pointers to gapbuffers
    document.buffer = malloc((INIT_DOC_SIZE) * sizeof(lineBuffer));

    document.start = &(document.buffer[0]);
    document.end = &(document.buffer[INIT_DOC_SIZE - 1]);

    document.used_lines = 0;
    document.size = INIT_DOC_SIZE;

    // for line in the string, create a line buffer, and store a pointer to
    // that line buffer in the buffer, in order in which they appear in the
    // document

    FILE *file = fopen(file_name, "r");

    if (!file) {
        file = fopen(file_name, "w");
        if (!file) {
            perror("error creating file");
            exit(1);
        }
    }

    while (1) {
        char tmp[5000];

        if (!fgets(tmp, 5000, file)) {
            break;
        }

        lineBuffer new = init_line(tmp);

        document.buffer[document.used_lines] = new;

        document.start++;
        document.used_lines++;

        // printf("%s", get_content_line(doc_buffer->buffer[0]));
    }

    // fill the remaining items with empty (unused) buffers
    for (int i = document.used_lines; i < INIT_DOC_SIZE; i++) {
        lineBuffer new = init_line(NULL);

        document.buffer[i] = new;
    }

    move_to_document_start(&document);

    return document;
}

void insert_line_doc(DocumentGapBuffer *document, char *content) {
    // we actually don't care about 'filled' items, we need to know what
    // lines are 'used' in the sense that they appear in the editor.
    //
    // we have *already* allocated the lines as large as INIT_DOC_SIZE. we
    // **do not** need to create a new line unless we have exceeded that line,
    // we can just mark it as in *use* by moving start and incremented used
    // lines.

    if (document->used_lines > document->size) {
        perror("growing");
        grow_document(document);
    }
    free(document->start->buffer);

    lineBuffer new_line = init_line(content);
    *document->start = new_line;

    document->start++;
    document->used_lines++;
}

void move_to_document_start(DocumentGapBuffer *document) {
    while (!(document->start == &(document->buffer[0]))) {
        move_gap_left_doc(document);
    }
}

void move_to_document_end(DocumentGapBuffer *document) {
    lineBuffer *end_offset = &(document->buffer[document->size - 1]);
    while (!(document->end == end_offset)) {
        move_gap_right_doc(document);
    }
}

lineBuffer *grab_line(DocumentGapBuffer *document, int desired_line) {
    /* we need the ability to be able to arbitrarily grab a 'used' line
     * check if &(document->buffer[line - 1]) is in range of buffer, if not
     * return it.
     *
     */
    assert(desired_line > 0);

    if (desired_line > document->used_lines) {
        move_to_document_end(document);

        assert(document->start > &(document->buffer[0]) &&
               document->start < &(document->buffer[document->size]));

        return (document->start - 1);
    }

    move_to_document_start(document);

    for (int i = 0; i < desired_line; i++) {
        move_gap_right_doc(document);
    }
    lineBuffer *line = &document->buffer[desired_line - 1];

    assert(line != NULL);

    return line;
}

lineBuffer *grab_next_line(DocumentGapBuffer *document, lineBuffer *line) {
    /*
     */
    lineBuffer *new_line = NULL;

    ptrdiff_t offset = document->end - document->start;

    new_line = line + 1;
    if (line >= document->start && line <= document->end) {
        new_line = line + offset;
    }

    assert(new_line != NULL);
    return new_line;
}
lineBuffer *grab_previous_line(DocumentGapBuffer *document, lineBuffer *line) {
    /*
     */
    lineBuffer *new_line = NULL;

    ptrdiff_t offset = document->end - document->start;

    new_line = line + 1;
    if (line >= document->start && line <= document->end) {
        new_line = line + offset;
    }

    assert(new_line != NULL);
    return new_line;
}

void move_gap_right_doc(DocumentGapBuffer *document) {
    lineBuffer *end_buffer = &(document->buffer[document->size - 1]);

    if (!(document->end == end_buffer)) {
        lineBuffer tmp = *(document->start);

        *(document->start) = *(document->end + 1);

        *(document->end + 1) = tmp;

        document->start++;
        document->end++;

    } else {
        printf("\nalready at end of buffer, cannot move right\n\n");
    }
}

void grow_document(DocumentGapBuffer *document) {
    int new_len = document->size + INIT_DOC_SIZE;

    lineBuffer *new_document = malloc(sizeof(lineBuffer) * new_len);
    printf("\n\n");
    dbg_doc_simple(document);
    for (int i = 0; i < new_len; i++) {
        if (i < document->size) {

            new_document[i] = document->buffer[i];
        } else {
            new_document[i] = init_line(NULL);
        }

        // once you find the start, make sure the new buffer gets the same
        // buffer start location
        if (document->start == &(document->buffer[i])) {
            document->start = &(new_document[i]);
        }
    }

    document->size += INIT_LINE_SIZE;
    document->end = &(new_document[document->size - 1]);

    free(document->buffer);

    document->buffer = new_document;
}

void move_gap_left_doc(DocumentGapBuffer *document) {

    if (!(document->start == &(document->buffer[0]))) {
        lineBuffer tmp = *(document->end);

        *(document->end) = *(document->start - 1);

        *(document->start - 1) = tmp;

        document->start--;
        document->end--;

    } else {
        printf("\nalready at start of buffer, cannot move left\n\n");
    }
}

void delete_document_line(DocumentGapBuffer *document) {
    if (!(document->start == &(document->buffer[0]))) {
        document->start--;
        document->used_lines--;
    } else {
        printf("doc at beg, cannot delete");
    }
}

void dbg_buffer_doc(DocumentGapBuffer *document) {
    // get_size will return actual numver of elements... e.g 300.
    // we index starting at zero... need -1
    int size = document->size;

    printf("size of doc: %d", size);

    for (int i = 0; i < size; i++) {
        printf("%s", document->buffer[i].buffer);
        printf("\n");
    };
}

void dbg_doc_simple(DocumentGapBuffer *document) {
    /* Prints a simple representation of the document where X represents a
     * 'filled' line, or a line that has been inserted into, while O's represent
     * writable allocated lines.
     */
    // get_size will return actual numver of elements... e.g 300.
    // we index starting at zero... need -1
    int size = document->size;

    int used_lines = document->used_lines;

    lineBuffer *line = &(document->buffer[0]);

    printf("printing simple doc illustration... | total size of doc: %d | "
           "filled items %d\n",
           size, used_lines);

    printf("[");
    for (int i = 0; i < size; i++) {
        if (line == document->start) {
            printf("{");
        }
        if (line >= document->start && line <= document->end) {
            printf("%c", 'O');
        } else {
            printf("%c", 'X');
        }

        if (line == document->end) {
            printf("}");
        }
        line++;
    };
    printf("]\n");
}

static int len_string(char *string) {
    /* Passing null pointers causes assert to fail, crashing program.
     * Passing non-null terminated strings results in undefined behavior.
     */

    // if string is NULL, assert fails and crashes program.
    assert(string);

    int i = 0;
    char *c = string;

    // while c, because null terminator is 0 or false
    while (*c) {
        i++;
        c++;
    }
    return i;
}
void print_content(DocumentGapBuffer *document) {
    int size = document->size;

    int used_line = 1; // lines are marked as 'used' by default,
                       // until we find the start of buffer, that marks
                       // beginning of 'unused' lines until the end of the
                       // buffer

    for (int i = 0; i < size; i++) {
        lineBuffer *line = &(document->buffer[i]);

        if (line == document->start) {
            used_line = 0;
        } else if (line > document->end) {
            used_line = 1;
        }
        int string_length = len_string(document->buffer[i].buffer);
        if (used_line) {
            write(STDOUT_FILENO, document->buffer[i].buffer, string_length);
            write(STDOUT_FILENO, "\r\n", 2);
        }
    }
}

void cleanup(DocumentGapBuffer *document) {
    /* Cleanup all the memory we allocated for all line buffers and the
     * document's buffer itself.
     */
    for (int i = 0; i < document->size; i++) {
        lineBuffer line = document->buffer[i];

        free(line.buffer);
    }
    free(document->buffer);
}

// int main(int argc, char *argv[]) {
//     DocumentGapBuffer document = init_document("../small_text.txt");
//
//     // dbg_doc_simple(&document);
//
//     // move_gap_left_doc(&document);
//     // move_gap_left_doc(&document);
//     // move_gap_left_doc(&document);
//
//     // dbg_doc_simple(&document);
//
//     lineBuffer *current_line = grab_line(&document, 3);
//
//     // dbg_doc_simple(&document);
//
//     insert_line_doc(&document, NULL);
//     current_line = grab_next_line(&document, current_line);
//     // dbg_doc_simple(&document);
//
//     insert_left_line(current_line, 'c');
//     insert_left_line(current_line, 'a');
//     insert_left_line(current_line, 'r');
//     insert_left_line(current_line, 's');
//     insert_left_line(current_line, ' ');
//     insert_left_line(current_line, 'g');
//     insert_left_line(current_line, 'o');
//     insert_left_line(current_line, ' ');
//
//     move_gap_left_doc(&document);
//
//     delete_document_line(&document);
//
//     dbg_doc_simple(&document);
//     for (int i = 0; i < 180; i++) {
//         insert_line_doc(&document, "hi");
//     }
//
//     dbg_doc_simple(&document);
//     insert_line_doc(&document, "HIHIHIHIHI");
//     dbg_doc_simple(&document);
//
//     for (int i = 0; i < 180; i++) {
//         delete_document_line(&document);
//     }
//
//     dbg_doc_simple(&document);
//     print_content(&document);
//
//     insert_line_doc(&document, "cat in ");
//     insert_line_doc(&document, "the hat ");
//     insert_line_doc(&document, "drove around ");
//     insert_line_doc(&document, "with a bat!");
//     dbg_doc_simple(&document);
//     print_content(&document);
//
//     move_to_document_start(&document);
//
//     cleanup(&document);
// }
