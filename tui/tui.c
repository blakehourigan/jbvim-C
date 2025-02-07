#include "tui.h"
#include "../cursor/cursor.h"
#include <asm-generic/ioctls.h>

#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

const int INFO_BAR_ROW_OFFSET = 1;
const int CURSOR_LOCATION_COL_OFFSET = 15;
const int EDITOR_MODE_COL_OFFSET = 10;

EditorInfoBar build_info() {
  struct winsize size;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &size);

  EditorInfoBar bar;

  bar.col_length = size.ws_col;
  bar.row = (size.ws_row) - INFO_BAR_ROW_OFFSET;
  bar.command_row = size.ws_row;
  bar.cursor_location_col = (size.ws_col) - CURSOR_LOCATION_COL_OFFSET;
  bar.editor_mode_col = EDITOR_MODE_COL_OFFSET;

  return bar;
};

void tui_enable_raw_mode(struct termios *termios_p,
                         struct termios *termios_og) {
  tcgetattr(STDIN_FILENO, termios_og);

  termios_p->c_iflag &=
      ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  termios_p->c_oflag &= ~OPOST;
  termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  termios_p->c_cflag &= ~(CSIZE | PARENB);
  termios_p->c_cflag |= CS8;
  tcsetattr(STDIN_FILENO, TCSANOW, termios_p);
}

void tui_disable_raw_mode(struct termios *termios_og) {
  tcsetattr(STDIN_FILENO, TCSANOW, termios_og);
}

void tui_enable_alternate_buffer() { write(STDOUT_FILENO, "\x1b[?1049h", 8); }

void tui_disable_alternate_buffer() { write(STDOUT_FILENO, "\x1b[?1049l", 8); }

void tui_setup() {
  cursor_set_background(5);

  EditorInfoBar info_bar = build_info();

  int len = info_bar.col_length;

  unsigned long size = sizeof(char) * len;

  char *bar = malloc(size);
  for (int i = 0; i < len; i++) {
    bar[i] = ' ';
  }
  int bar_row = info_bar.row;
  cursor_move_to(bar_row, 1);
  write(STDOUT_FILENO, bar, size);

  cursor_move_home();

  Cursor *curs = malloc(sizeof(Cursor));
  cursor_get_coords(curs);

  char coords_str[] = "( , )";
  size_t coords_size =
      sizeof(coords_str) + sizeof(curs->col) + sizeof(curs->line);

  char *coords = malloc(size);
  snprintf(coords, size, "(%d, %d)", curs->col, curs->line);

  cursor_move_to(info_bar.row, info_bar.cursor_location_col);
  write(STDOUT_FILENO, coords, coords_size);

  cursor_set_background(0);

  // cleanup
  free(curs);
  free(coords);
  free(bar);
}
