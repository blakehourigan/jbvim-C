#include "cursor.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

static void sort_response(Cursor *curs, char *s) {
    int len = sizeof(s);
    int row = 0;

    int read_row = 0;
    int read_col = 0;

    int acc = 0;

    for (int i = 0; i < len; i++) {
        if (s[i] == '[') {
            read_row = 1;
            continue;
        }

        if (read_row) {
            if (s[i] == ';') {
                read_row = 0;
                read_col = 1;
                curs->line = acc;
                acc = 0;
                continue;
            }
            int num = s[i] - '0';
            acc *= 10;
            acc += num;
        }

        if (read_col) {
            if (s[i] == 'R') {
                read_col = 0;
                curs->col = acc;
                acc = 0;
                continue;
            }
            int num = s[i] - '0';
            acc *= 10;
            acc += num;
        }
    }
}

void cursor_get_coords(Cursor *curs) {
    char *s = malloc(sizeof(char) * 30);
    char buf;
    int i = 0;
    int start = 0;

    // request the information from the terminal
    write(STDOUT_FILENO, "\x1b[6n", 4);

    while (1) {
        read(STDIN_FILENO, &buf, 1);
        if (start) {
            s[i] = buf;
            i++;
        }
        if (buf == 'R') {
            start = 0;
            break;
        }

        if (buf == '\x1b') {
            start = 1;
        }
    };

    sort_response(curs, s);

    // cleanup
    free(s);
}

void cursor_enable_bar_cursor() { write(STDOUT_FILENO, "\x1b[6 q", 5); }
void cursor_enable_standard_cursor() { write(STDOUT_FILENO, "\x1b[0 q", 5); }

void cursor_move_right() { write(STDOUT_FILENO, "\x1b[1C", 4); }
void cursor_move_left() { write(STDOUT_FILENO, "\x1b[1D", 4); }
void cursor_move_up() { write(STDOUT_FILENO, "\x1b[1A", 4); }
void cursor_move_down() { write(STDOUT_FILENO, "\x1b[1B", 4); }

void cursor_move_to(int line, int column) {
    unsigned long size = sizeof("\x1b[;f") + sizeof(line) + sizeof(column);
    char *s = malloc(size);
    snprintf(s, size, "\x1b[%d;%df", line, column);
    write(STDOUT_FILENO, s, size);

    // cleanup
    free(s);
}
void cursor_move_home() { write(STDOUT_FILENO, "\x1b[H", 3); }

void cursor_set_foreground(int color) {
    unsigned long size = sizeof("\x1b[38;5;m") + sizeof(color);
    char *s = malloc(size);
    snprintf(s, size, "\x1b[38;5;%dm", color);
    write(STDOUT_FILENO, s, size);

    // cleanup
    free(s);
}
void cursor_set_background(int color) {
    unsigned long size = sizeof("\x1b[48;5;m") + sizeof(color);
    char *s = malloc(size);
    snprintf(s, size, "\x1b[48;5;%dm", color);
    write(STDOUT_FILENO, s, size);

    // cleanup
    free(s);
}

void cursor_delete_end_of_line() { write(STDOUT_FILENO, "\x1b[0K", 4); }
void cursor_return_newline(Cursor *curs) {
    cursor_get_coords(curs);
    cursor_move_to(curs->line + 1, curs->col);
}
void cursor_reset_modes() { write(STDOUT_FILENO, "\x1b[0m", 3); }
void cursor_save_cursor_position() { write(STDOUT_FILENO, "\x1b[s", 3); }
void cursor_restore_cursor_position() { write(STDOUT_FILENO, "\x1b[u", 3); }
void cursor_make_invisible() { write(STDOUT_FILENO, "\x1b[?25h", 6); }
void cursor_make_visible() { write(STDOUT_FILENO, "\x1b[28m", 5); }

void cursor_backspace() {
    write(STDOUT_FILENO, "\b", 1);
    cursor_move_left();
    write(STDOUT_FILENO, " ", 1);
}

void cursor_write_char(char character) { write(STDOUT_FILENO, &character, 1); }

void delay(int seconds) {
    unsigned int ret_time = time(0) + seconds;
    while (time(0) < ret_time) {
    }
}
