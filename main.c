#include "cursor/cursor.h"
#include "gap_buffer/gap_buffer.h"
#include "tui/tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

enum EditorMode {
  Normal,
  Insert,
};

int main() {
  struct termios termios_p;
  struct termios termios_og;

  tui_enable_alternate_buffer();
  tui_enable_raw_mode(&termios_p, &termios_og);
  fflush(stdin);
  fflush(stdout);

  cursor_move_home();

  char str[] = "hello, world!";

  GapBuffer *buffer = gap_init(str);
  int len_str = str_len(str);

  for (int i = 0; i < len_str; i++) {
    write(STDOUT_FILENO, &buffer->buffer[i], sizeof(buffer->buffer[i]));
  }

  tui_setup();
  cursor_move_home();

  // begin main loop
  while (1) {
    // refresh_tui();
    cursor_move_home();

    // reading input from the user
    char input;
    read(STDIN_FILENO, &input, 1);
    write(STDOUT_FILENO, &input, 1);

    if (input == 'q') {
      tui_disable_alternate_buffer();
      tui_disable_raw_mode(&termios_og);
      free(buffer->buffer);
      free(buffer);
      return 0;
    }
    input = '\0';
  }

  return 0;
}
