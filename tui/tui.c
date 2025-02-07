#include "tui.h"
#include <asm-generic/ioctls.h>
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
