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

static void tui_draw_info_bar(EditorInfoBar *info_bar) {
  cursor_save_cursor_position();

  cursor_set_background(5);

  int len = info_bar->col_length;

  unsigned long size = sizeof(char) * len;

  char *bar = malloc(size);
  for (int i = 0; i < len; i++) {
    bar[i] = ' ';
  }
  int bar_row = info_bar->row;
  cursor_move_to(bar_row, 1);
  write(STDOUT_FILENO, bar, size);

  // cleanup
  free(bar);
  cursor_set_background(0);
  cursor_restore_cursor_position();
}

static void tui_update_curs_pos(EditorInfoBar *info_bar) {
  cursor_set_background(5);

  Cursor *curs = malloc(sizeof(Cursor));
  cursor_get_coords(curs);

  char coords_str[] = "( , )";
  size_t coords_size =
      sizeof(coords_str) + sizeof(curs->col) + sizeof(curs->line);

  char *coords = malloc(coords_size);
  snprintf(coords, coords_size, "(%d, %d)", curs->col, curs->line);

  cursor_move_to(info_bar->row, info_bar->cursor_location_col);
  write(STDOUT_FILENO, coords, coords_size);

  // cleanup
  free(curs);
  free(coords);

  cursor_set_background(0);
}

void tui_setup(struct termios *termios_og) {
  struct termios termios_p;
  // setup tui

  tui_enable_alternate_buffer();
  tui_enable_raw_mode(&termios_p, termios_og);
  fflush(stdin);
  fflush(stdout);

  cursor_move_home();

  EditorInfoBar info_bar = build_info();

  tui_draw_info_bar(&info_bar);

  tui_update_curs_pos(&info_bar);

  cursor_set_background(0);
  cursor_move_home();
}

static void tui_draw_mode(EditorInfoBar *info_bar, enum EditorMode *mode) {
  cursor_set_foreground(5);

  char mode_str[7];

  if (*mode == Normal) {
    char str[] = "Normal";
    for (int i = 0; i < sizeof(str); i++) {
      mode_str[i] = str[i];
    }
  } else if (*mode == Insert) {
    char str[] = "Insert";
    for (int i = 0; i < sizeof(str); i++) {
      mode_str[i] = str[i];
    }
  } else if (*mode == Command) {
    char str[] = "Command";
    for (int i = 0; i < sizeof(str); i++) {
      mode_str[i] = str[i];
    }
  }

  size_t str_size = sizeof(mode_str);

  cursor_move_to(info_bar->command_row, info_bar->editor_mode_col);
  write(STDOUT_FILENO, mode_str, str_size);
  cursor_set_foreground(15);
}

void tui_move_to_command(EditorInfoBar *info_bar) {
  cursor_set_foreground(5);
  cursor_save_cursor_position();

  cursor_move_to(info_bar->command_row, 1);
  write(STDOUT_FILENO, ":", 1);
}
void tui_exit_command(EditorInfoBar *info_bar, Cursor *prev_curs_pos) {
  cursor_set_foreground(15);
  cursor_set_background(0);

  int len = info_bar->col_length;

  cursor_move_to(info_bar->command_row, 1);

  unsigned long size = sizeof(char) * len;

  char *bar = malloc(size);
  for (int i = 0; i < len; i++) {
    bar[i] = ' ';
  }
  int bar_row = info_bar->command_row;
  cursor_move_to(bar_row, 1);
  write(STDOUT_FILENO, bar, size);

  int line = prev_curs_pos->line;
  int col = prev_curs_pos->col;
  cursor_move_to(line, col);

  // cleanup
  free(bar);
}

void tui_refresh(enum EditorMode *mode) {
  cursor_save_cursor_position();
  EditorInfoBar info_bar = build_info();

  tui_draw_info_bar(&info_bar);
  tui_update_curs_pos(&info_bar);
  tui_draw_mode(&info_bar, mode);

  switch (*mode) {
  case Normal:
    break;
  case Insert:
    break;
  case Command:
    break;
  default:
    break;
  }
  cursor_restore_cursor_position();
}
