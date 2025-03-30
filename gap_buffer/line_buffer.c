#include "line_buffer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

void print_buffer(lineBuffer *line) {
    /* If line is NULL, assert fails, program crashes.
     *
     */
    assert(line);

    printf("printing buffer... | size: %d | filled items %d\n\n", line->size,
           line->filled_items);

    printf("[");

    for (int i = 0; i < line->size; i++) {
        char *c = &(line->buffer[i]);
        if (c == line->start) {
            printf("{");
            printf("%c", *c);
            continue;
        }
        if (c == line->end) {
            printf("%c", *c);
            printf("}");
            continue;
        }

        if (*c == '\0') {
            printf("0");
        } else {
            printf("%c", *c);
        }
    }
    printf("]");
    printf("\n");
}

lineBuffer init(char *line_content) {
    // by avoiding the allocation of a heap object here and using struct copying
    // instead, we trade complexity for a slight hit in speed, however copying 3
    // pointers is really fast, so not a huge deal **in this case**
    lineBuffer line;

    line.buffer = malloc(sizeof(char) * INIT_LINE_SIZE);
    line.start = &line.buffer[0];
    line.end = &line.buffer[INIT_LINE_SIZE - 1];

    line.size = INIT_LINE_SIZE;

    int string_len = 0;

    if (line_content) {
        string_len = len_string(line_content);

        for (int i = 0; i < string_len; i++) {
            insert_left_line(&line, line_content[i]);
        }
    }

    for (int i = string_len; i < INIT_LINE_SIZE; i++) {
        line.buffer[i] = '\0';
    }

    print_buffer(&line);

    return line;
}

void insert_left_line(lineBuffer *line, char character) {
    if (line->filled_items < line->size) {

        *(line->start) = character;

        line->start++;
        line->filled_items++;
    } else {
        grow_line(line);
        print_buffer(line);

        *(line->start) = character;

        line->start++;
        line->filled_items++;
    }
}

void move_gap_right_line(lineBuffer *line) {
    /* if the line's end pointer (pointer to end of writable buffer) is
     * equivalent to the very end of the total buffer (written and writable)
     * then the gap is as far right as you can go, do not move right further
     */
    char *end_buffer = &(line->buffer[line->size - 1]);

    if (!(line->end == end_buffer)) {
        // move
        // get the character at end of buffer + 1 (char after end of writable
        // buffer)
        *(line->start) = *(line->end + 1);

        line->start++;
        line->end++;

        *(line->end) = '\0';
    } else {
        printf("can't do that anymore\n");
    }
}

void move_gap_left_line(lineBuffer *line) {
    if (!(line->start == &(line->buffer[0]))) {
        *(line->end) = *(line->start - 1);

        line->end--;
        line->start--;
    } else {
        printf("can't do that anymore");
    }
}

void delete_item_line(lineBuffer *line) {
    if (!(line->start == &(line->buffer[0]))) {
        line->start--;
    } else {
        printf("can't do that anymore");
    }
}

void move_to_line_start(lineBuffer *line) {
    while (!(line->start == &(line->buffer[0]))) {
        *(line->end) = *(line->start - 1);

        line->end--;
        line->start--;
    }
}
int move_to_line_end(lineBuffer *line) {
    // utilized line->filled items to find the position of last character
    // inserted
    move_to_line_start(line);

    // keep track of moves so that GUI knows where to move to.
    int moves = 0;

    for (int i = 0; i < line->filled_items; i++) {
        move_gap_right_line(line);
    }
    return moves;
}

void grow_line(lineBuffer *line) {
    /* if we attempt an insertion but filled items == size, we do not have
     * enough room for another element, we need to grow such that line->start
     * remains where it is and line->end is extended 150 more characters out.
     *
     * every time that we expand we increase size by INIT_LINE_SIZE
     */

    char *new_buffer = malloc(sizeof(char) * (line->size + INIT_LINE_SIZE));

    for (int i = 0; i < line->size; i++) {
        new_buffer[i] = line->buffer[i];

        if (line->start == &(line->buffer[i])) {
            printf("setting new start");
            line->start = &(new_buffer[i]);
        }
    }

    for (int i = line->size; i < line->size + INIT_LINE_SIZE; i++) {
        if (line->start == &(line->buffer[i])) {
            printf("setting new start");
            line->start = &(new_buffer[i]);
        }
        new_buffer[i] = '\0';
    }

    line->size += INIT_LINE_SIZE;
    line->end = &(new_buffer[line->size - 1]);

    free(line->buffer);

    line->buffer = new_buffer;
}

int main(int argc, char *argv[]) {
    // lineBuffer line = init(NULL);
    lineBuffer line = init("hello, world!");

    char insert[] = "sad ";

    print_buffer(&line);

    for (int i = 0; i < 200; i++) {
        insert_left_line(&line, 'c');
    }
    print_buffer(&line);

    // move_to_line_start(&line);

    // print_buffer(&line);

    // move_to_line_end(&line);

    // print_buffer(&line);

    // for (int i = 0; i < 6; i++) {
    //     move_gap_left_line(&line);
    //     print_buffer(&line);
    // }

    // int insertion = len_string(insert);

    // for (int i = 0; i < insertion; i++) {
    //     insert_left_line(&line, insert[i]);
    // }

    // for (int i = 0; i < 6; i++) {
    //     move_gap_right_line(&line);
    //     print_buffer(&line);
    // }

    // free the buffer held inside of the line
    free(line.buffer);

    return 1;
}
