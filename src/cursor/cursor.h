#include <stdio.h>
#include <unistd.h>

#ifndef CURSOR_H
#define CURSOR_H

typedef struct Cursor {
    int line;
    int col;
} Cursor;

// Cursor *build_cursor(int line, int col);

/// constructs new instance of cursor with current and curosor row and col
/// using ascii escape character "\x1b[6n". stdout is flushed and stdin
/// response from terminal is read into cursor instance.
void cursor_get_coords(Cursor *curs);

void cursor_enable_bar_cursor();
void cursor_enable_standard_cursor();

void cursor_move_right();
void cursor_move_left();
void cursor_move_up();
void cursor_move_down();

void cursor_move_to(int line, int column);
void cursor_move_home();

void cursor_return_newline(Cursor *curs);

void cursor_save_cursor_position();

void cursor_restore_cursor_position();

void cursor_backspace();

void cursor_write_char(char character);

void cursor_set_foreground(int color);

void cursor_set_background(int color);

void cursor_delete_end_of_line();

void cursor_reset_modes();

void cursor_make_invisible();

void cursor_make_visible();

#endif
