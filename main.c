#include "cursor/cursor.h"
#include "editor_common.h"
#include "gap_buffer/gap_buffer.h"
#include "tui/tui.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

const size_t COMMAND_SIZE = 50;

void main_delay(int seconds) {
  unsigned int ret_time = time(0) + seconds;
  while (time(0) < ret_time) {
  }
}

void basic_movement_handler(unsigned char *input, struct EditorState *state) {
  // <C-c> | esc| arrows
  // 3 | 27 | 183 | 184 | 185 | 186
  char error_msg[] = "keyboard shortcut not found";

  int input_num = *input;

  switch (*input) {
  case 3:
  case 27:
    state->mode = Normal;
    tui_refresh(&state->mode);
    break;
    // up arrow or k key
  case 'k':
  case 183:
    cursor_move_up();
    break;
    // down arrow or j key
  case 'j':
  case 184:
    cursor_move_down();
    break;
    // right arrow or l key
  case 'l':
  case 185:
    cursor_move_right();
    break;
    // left arrow or h key
  case 'h':
  case 186:
    cursor_move_left();
    break;
  default:
    write(STDOUT_FILENO, error_msg, sizeof(error_msg));
  }
}

void handle_insert_mode(unsigned char *input, struct EditorState *state) {
  switch (*input) {
  case 3:
  case 27:
    state->mode = Normal;
    tui_refresh(&state->mode);
    break;
  default:
    write(STDOUT_FILENO, input, 1);
    break;
  }
}
void handle_normal_mode(unsigned char *input, struct EditorState *state,
                        EditorInfoBar *info_bar) {

  switch (*input) {
  case 3:
  case 27:
    // up arrow or k key
  case 'k':
  case 183:
    // down arrow or j key
  case 'j':
  case 184:
    // right arrow or l key
  case 'l':
  case 185:
    // left arrow or h key
  case 'h':
  case 186:
    basic_movement_handler(input, state);
    break;
  case 'i':
    state->mode = Insert;
    tui_refresh(&state->mode);
    break;
  case ':':
    state->mode = Command;
    cursor_get_coords(&state->previous_curs_pos);
    tui_move_to_command(info_bar);
    tui_refresh(&state->mode);
    break;
  default:
    break;
  }
}
void process_command(struct EditorState *state, char *command,
                     GapBufferDoc *buffer) {
  int i = 0;
  char c = command[0];
  while (1) {
    char c = command[i];
    switch (c) {
    case 'q':
      tui_disable_alternate_buffer();
      tui_disable_raw_mode(&state->og_settings);
      // cleanup and exit the program with noramal exit status
      free(command);
      free(buffer->buffer);
      free(buffer);
      exit(0);
      break;
    default:
      break;
    }
    if (i >= COMMAND_SIZE) {
      break;
    }
    i++;
  }
}
void handle_command_mode(unsigned char *input, struct EditorState *state,
                         GapBufferDoc *buffer, EditorInfoBar *info_bar) {
  char *command = malloc(sizeof(char) * COMMAND_SIZE);
  if (!command) {
    return;
  }
  // process the first char passed in from main
  command[0] = input[0];
  write(STDOUT_FILENO, input, 1);

  int i = 1;
  char command_input;
  int run = 1;
  while (run) {
    if ((read(STDIN_FILENO, &command_input, 1)) > 0) {
      write(STDOUT_FILENO, &command_input, 1);
      switch (command_input) {
        // handle ascii for enter key
      case 13:
        command[i] = '\0';
        process_command(state, command, buffer);
        break;
      case 3:
      case 27:
        state->mode = Normal;
        tui_exit_command(info_bar, &state->previous_curs_pos);
        int line = state->previous_curs_pos.line;
        int col = state->previous_curs_pos.col;
        cursor_move_to(line, col);
        run = 0;
        tui_refresh(&state->mode);
        break;
      default:
        command[i] = command_input;
        i++;
        break;
      }
    }
  }

  free(command);
}

int main(int argc, char *argv[]) {
  struct termios termios_og;
  struct Cursor previous_cursor_pos;
  struct EditorState *state = malloc(sizeof(struct EditorState));

  state->mode = Normal;
  state->og_settings = termios_og;
  state->previous_curs_pos = previous_cursor_pos;

  EditorInfoBar info_bar = build_info();

  // open file given by command args

  // read exiting file if it exists

  // GapBuffer *buffer = gap_init_line(str);
  char *file_name = malloc(sizeof(char) * 150);
  if (argc > 1) {
    file_name = argv[1];
  } else {
    file_name = "tmp.txt";
  }
  GapBufferDoc *buffer = gap_init_doc(file_name);

  tui_setup(&termios_og);

  // writing exiting line contents to the screen
  write_content(buffer, 0, 20);
  cursor_move_home();

  // begin main loop
  while (1) {
    // reading input from the user
    char input = '\0';
    unsigned char full_input = '\0';

    if (read(STDIN_FILENO, &input, 1) > 0) {
      if (input == '\x1b') {
        full_input += input;
        while ((read(STDIN_FILENO, &input, 1) > 0)) {
          full_input += input;
        }
      } else {
        full_input = input;
      }

      tui_refresh(&state->mode);
      switch (state->mode) {
      case Normal:
        // pass in the input character and decide whether to change modes,
        // move cursor, etc
        handle_normal_mode(&full_input, state, &info_bar);
        break;
      case Insert:
        // pass in character pressed and process whether we should change
        // modes or insert the character, move line contents, etc
        handle_insert_mode(&full_input, state);
        break;
      case Command:
        // pass the initial input char into command mode and hand off
        // control until command is submitted OR mode is changed
        handle_command_mode(&full_input, state, buffer, &info_bar);
        break;
      default:
        break;
      }
    };
  }
  return 0;
}
