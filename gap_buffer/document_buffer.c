#include "document_buffer.h"
#include "line_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void insert_left_doc(DocumentGapBuffer *document, lineBuffer line) {
    if (document->filled_items < document->size) {
        *document->start = line;

        document->start++;
    } else {
        perror("growing");
        // grow_buffer_doc(document);
    }
}

DocumentGapBuffer init_document(char *file_name) {
    DocumentGapBuffer document;

    // enough room for 150 (initially) pointers to gapbuffers
    document.buffer = malloc((INIT_DOC_SIZE) * sizeof(lineBuffer));

    document.start = &(document.buffer[0]);
    document.end = &(document.buffer[INIT_DOC_SIZE - 1]);

    document.filled_items = 0;
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

        insert_left_doc(&document, new);
        // printf("%s", get_content_line(doc_buffer->buffer[0]));
        document.filled_items += 1;
    }

    // fill the remaining items so we have no null references
    for (int i = document.filled_items; i < INIT_DOC_SIZE; i++) {
        lineBuffer new = init_line(NULL);

        document.buffer[i] = new;
    }

    return document;
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
    // get_size will return actual numver of elements... e.g 300.
    // we index starting at zero... need -1
    int size = document->size;
    int filled_items = document->filled_items;
    lineBuffer *line = &(document->buffer[0]);

    printf("printing simple doc illustration... | total size of doc: %d | "
           "filled items %d\n",
           size, filled_items);

    for (int i = 0; i < size; i++) {
        if (line == document->start) {
            printf("{");
        }

        if (i < document->filled_items) {
            printf("%c", 'X');
        } else {
            printf("%c", 'O');
        }

        if (line == document->end) {
            printf("}");
        }
        line++;
    };
}

void print_content(DocumentGapBuffer *document) {
    int filled_items = document->filled_items;

    for (int i = 0; i < filled_items; i++) {
        printf("%s", document->buffer[i].buffer);
        printf("\n");
    };
}

int main(int argc, char *argv[]) {
    DocumentGapBuffer document = init_document("../small_text.txt");

    print_content(&document);
    dbg_doc_simple(&document);

    for (int i = 0; i < document.size; i++) {
        lineBuffer line = document.buffer[i];

        free(line.buffer);
    }
    free(document.buffer);
}
