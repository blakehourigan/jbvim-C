#include "document_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int str_len(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

static int get_size_line(GapBuffer *buffer) {
    int i = 0;
    while (1) {
        if (i > 1000) {
            perror("couldn't find the len");
            break;
        }
        if (&buffer->buffer[i] == buffer->end) {
            i++;
            break;
        } else {
            i++;
        }
    }
    // include the newline in the len
    return i;
}
static int get_size_doc(GapBufferDoc *doc) {
    int i = 0;
    while (1) {
        if (i > 1000) {
            perror("couldn't find the len");
            break;
        }
        if (&doc->buffer[i] == doc->end) {
            i++;
            break;
        } else {
            i++;
        }
    }
    return i;
}
GapBuffer *gap_init_line(char *str) {

    GapBuffer *buffer = malloc(sizeof(GapBuffer) + 150 * sizeof(char));
    buffer->buffer = malloc(150 * sizeof(char));

    buffer->start = &buffer->buffer[0];
    buffer->end = &buffer->buffer[INIT_GAP_END];
    *(buffer->end) = '\n';
    buffer->end--;
    buffer->filled_items = 0;

    // printf("\n%B", str == NULL);
    if (!(str == NULL)) {
        int len_str = str_len(str);

        for (int i = 0; i < len_str; i++) {
            insert_left_line(buffer, str[i]);
            buffer->filled_items += 1;
        }
    }

    return buffer;
};

void move_gap_right_line(GapBuffer *buffer) {
    if (buffer->start == buffer->end) {
        return;
    } else {
        char tmp = *buffer->start;
        *buffer->start = *(buffer->end + 1);
        *buffer->end = tmp;

        buffer->start++;
        buffer->end++;
    }
}

void move_gap_left_line(GapBuffer *buffer) {
    // if this is the first item in the buffer
    if (buffer->start == &buffer->buffer[0]) {
        return;
    } else {
        char tmp = *(buffer->start - 1);
        *buffer->end = tmp;

        buffer->start--;
        buffer->end--;
    }
}

void grow_buffer_line(GapBuffer *buffer) {
    int cur_size = get_size_line(buffer);
    int des_size = cur_size + MAX_CAPACITY_INIT;
    char *new_array =
        malloc((sizeof(char) * cur_size) + (MAX_CAPACITY_INIT * sizeof(char)));

    for (int i = 0; i < cur_size + MAX_CAPACITY_INIT; i++) {
        if (&buffer->buffer[i] < buffer->start) {
            new_array[i] = buffer->buffer[i];
        } else if (buffer->start < &buffer->buffer[i] &&
                   &buffer->buffer[i] < buffer->end) {
        } else if (&buffer->buffer[i] > buffer->end) {
            new_array[i] = buffer->buffer[i];
        }
    }
    // free up the old allocation, assign new array to the ptr
    free(buffer->buffer);
    buffer->buffer = new_array;
    buffer->end += MAX_CAPACITY_INIT;
}

void insert_left_line(GapBuffer *buffer, char item) {
    if (buffer->start == buffer->end) {
        perror("caution, growing");
        perror("caution, growing");
        perror("caution, growing");
        grow_buffer_line(buffer);
    }
    *buffer->start = item;

    buffer->start++;
}

void delete_item_line(GapBuffer *buffer) {
    if (buffer->start != &buffer->buffer[0]) {
        *buffer->start = '\0';
    }
    buffer->start -= 1;
    buffer->filled_items -= 1;
}

int is_last_char(GapBuffer *buffer) {
    dbg_buffer(buffer, 15);
    // if (*(buffer->end) == '\n') {
    //   return 1;
    // } else {
    //   return 0;
    // }
}

char *get_content_line(GapBuffer *buffer) {
    int size = get_size_line(buffer);
    char *str = malloc(sizeof(char) * size);

    for (int i = 0; i < size; i++) {
        str[i] = buffer->buffer[i];
    }
    return str;
}

int get_len_doc(GapBufferDoc *doc) {
    int i = 0;
    while (&doc->buffer[i] != doc->end) {
        i++;
    }
    return i++;
}

void write_content(GapBufferDoc *doc, int start, int end) {

    for (int i = start; i < end; i++) {
        GapBuffer line_buffer = doc->buffer[i];
        int line_sz = get_size_line(&line_buffer);

        for (int j = 0; j < line_sz + 1; j++) {
            if (line_buffer.buffer[j] == '\n') {
                write(STDOUT_FILENO, "\r\n", 2);
                break;
            } else {
                write(STDOUT_FILENO, &line_buffer.buffer[j], 1);
            }
        }
        if ((&line_buffer + 1) == doc->start) {
            // print warning
            // tui_write_warning
            // perror("attempt to read past filled items in doc buffer, breaking
            // early");
            break;
        }
    }
}

static void reset_doc(GapBufferDoc *doc) {
    int filled_items = doc->filled_items;
    for (int i = 0; i < filled_items; i++) {
        move_gap_left_doc(doc);
    }
}

void delete_item_doc(GapBufferDoc *doc) {
    if (doc->start != &doc->buffer[0]) {
        GapBuffer *new = gap_init_line(NULL);
        *(doc->start - 1) = *new;
        doc->start--;
        doc->filled_items--;
    }
}

GapBufferDoc *gap_init_doc(char *file_name) {
    GapBufferDoc *doc_buffer = malloc(sizeof(GapBufferDoc));

    // enough room for 150 (initially) pointers to gapbuffers
    doc_buffer->buffer = malloc(MAX_CAPACITY_INIT * sizeof(GapBuffer *));

    doc_buffer->start = &(doc_buffer->buffer[0]);
    doc_buffer->end = &(doc_buffer->buffer[INIT_GAP_END]);

    doc_buffer->filled_items = 0;

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

    char c;
    int i = 0;

    char *line_buf = malloc(sizeof(char) * 300);

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            GapBuffer *new = gap_init_line(line_buf);
            insert_left_doc(doc_buffer, new);
            // printf("%s", get_content_line(doc_buffer->buffer[0]));
            doc_buffer->filled_items += 1;
            // clear the string so we dont get weird repeating lines and such
            memset(line_buf, 0, str_len(line_buf));
            i = 0;
            continue;
        }
        line_buf[i] = c;
        i++;
    }

    for (int i = doc_buffer->filled_items; i < MAX_CAPACITY_INIT; i++) {
        GapBuffer *new = gap_init_line(NULL);
        insert_left_doc(doc_buffer, new);
    }

    return doc_buffer;
}

void move_gap_right_doc(GapBufferDoc *buffer) {
    if (buffer->start == buffer->end) {
        return;
    } else {
        GapBuffer *tmp = (buffer->start);
        *buffer->start = *(buffer->end + 1);
        buffer->end = tmp;

        buffer->start++;
        buffer->end++;
    }
}

void move_gap_left_doc(GapBufferDoc *buffer) {
    // if this is the first item in the buffer
    if (buffer->start == &buffer->buffer[0]) {
        return;
    } else {
        GapBuffer *tmp = (buffer->start - 1);
        buffer->end = tmp;

        buffer->start--;
        buffer->end--;
    }
}

void insert_left_doc(GapBufferDoc *doc, GapBuffer *item) {
    if (doc->start == doc->end) {
        perror("caution, growing");
        grow_buffer_doc(doc);
    }
    doc->start = item;

    doc->start++;
}

void grow_buffer_doc(GapBufferDoc *doc) {
    int cur_size = get_size_doc(doc);
    int des_size = cur_size + MAX_CAPACITY_INIT;

    GapBuffer *new_buffer = malloc(des_size * sizeof(GapBuffer *));

    for (int i = 0; i < cur_size - 1; i++) {
        new_buffer[i] = doc->buffer[i];
    }

    for (int i = cur_size - 1; i < des_size; i++) {
        GapBuffer *new = gap_init_line(NULL);
        new_buffer[i] = *new;
    }

    // free up the old buf space, assign new array to the ptr
    free(doc->buffer);

    doc->buffer = new_buffer;
    doc->start = &doc->buffer[cur_size];
    doc->end = &doc->buffer[des_size];
}

void dbg_buffer(GapBuffer *buffer, int buf_num) {
    // get_size will return actual numver of elements... e.g 300.
    // we index starting at zero... need -1
    int size = get_size_line(buffer) - 1;
    printf("buf number is: %d\n", buf_num);

    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            printf("(%d: %s)*****]", i, "newline");
        } else if (&buffer->buffer[i] == buffer->start) {
            printf("[****(%d: %c)", i, buffer->buffer[i]);
        } else if (&buffer->buffer[i] == buffer->end) {
            printf("(%d: %c)*****]", i, buffer->buffer[i]);
        } else if ((i - 1) % 20 == 0 && (i != 1)) {
            printf("\n");
        } else {
            printf("(%d: %c)", i, buffer->buffer[i]);
        }
    };
}
void dbg_buffer_doc(GapBufferDoc *doc) {
    // get_size will return actual numver of elements... e.g 300.
    // we index starting at zero... need -1
    int size = get_size_doc(doc) - 1;

    printf("size of doc: %d", size);
    for (int i = 0; i < size; i++) {
        dbg_buffer(&(doc->buffer[i]), i);
        printf("\n\n");
    };
}

int main() {
    GapBufferDoc *doc = gap_init_doc("../small_text.txt");

    dbg_buffer_doc(doc);

    GapBuffer buf = doc->buffer[20];
    dbg_buffer(&buf, 20);

    printf("%B", buf.buffer[100] == '\0');

    // write_content(doc, 0, doc_len);
}
