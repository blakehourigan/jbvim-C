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

void enable_raw_mode(struct termios *termios_p, struct termios *termios_og) {
  tcgetattr(STDIN_FILENO, termios_og);

  termios_p->c_iflag &=
      ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  termios_p->c_oflag &= ~OPOST;
  termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  termios_p->c_cflag &= ~(CSIZE | PARENB);
  termios_p->c_cflag |= CS8;
  tcsetattr(STDIN_FILENO, TCSANOW, termios_p);
}

void disable_raw_mode(struct termios *termios_og) {
  tcsetattr(STDIN_FILENO, TCSANOW, termios_og);
}

void enable_alternate_buffer() { write(STDOUT_FILENO, "\x1b[?1049h", 8); }

void disable_alternate_buffer() { write(STDOUT_FILENO, "\x1b[?1049l", 8); }

void setup_tui() {
  set_background(2);

  EditorInfoBar info_bar = build_info();

  int len = info_bar.col_length;

  unsigned long size = sizeof(char) * len;

  char *bar = malloc(size);
  for (int i = 0; i < len; i++) {
    bar[i] = ' ';
  }

  int bar_row = info_bar.row;
  move_cursor_to(bar_row, 1);

  write(STDOUT_FILENO, bar, size);
}

int main() {
  struct termios termios_p;
  struct termios termios_og;

  enable_alternate_buffer();
  enable_raw_mode(&termios_p, &termios_og);
  fflush(stdin);
  fflush(stdout);
  move_home();

  char str[] = "hello, world!";

  GapBuffer *buffer = gap_init(str);
  int len_str = str_len(str);

  for (int i = 0; i < len_str; i++) {
    write(STDOUT_FILENO, &buffer->buffer[i], sizeof(buffer->buffer[i]));
  }

  setup_tui();
  move_home();
  while (1) {
    char input;
    read(STDIN_FILENO, &input, 1);

    write(STDOUT_FILENO, &input, 1);

    if (input == 'q') {
      disable_alternate_buffer();
      disable_raw_mode(&termios_og);
      free(buffer->buffer);
      free(buffer);
      return 0;
    }
    input = '\0';
  }

  return 0;
}
