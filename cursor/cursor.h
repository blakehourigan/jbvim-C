#include <stdio.h>
#include <unistd.h>

typedef struct {
  int line;
  int col;
} Cursor;

// Cursor *build_cursor(int line, int col);

/// constructs new instance of cursor with current and curosor row and col
/// using ascii escape character "\x1b[6n". stdout is flushed and stdin
/// response from terminal is read into cursor instance.
void get_cursor_coords(Cursor *curs);

void enable_bar_cursor();
void enable_standard_cursor();
void move_right(int num);
void move_left(int num);
void move_up(int num);

void move_down(int num);

void move_cursor_to(int line, int column);
void move_home();

void return_newline(Cursor *curs);

void save_cursor_position();

void restore_cursor_position();

void backspace();

void write_char(char character);

void set_foreground(int color);

void set_background(int color);

void delete_end_of_line();

void reset_modes();

void make_invisible();

void make_visible();
