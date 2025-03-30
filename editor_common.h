#include "cursor/cursor.h"
#include <termios.h>

#ifndef EDITOR_COMMON_H
#define EDITOR_COMMON_H

enum EditorMode {
  Normal,
  Insert,
  Command,
};

typedef struct EditorState {
  enum EditorMode mode;
  struct termios og_settings;
  Cursor previous_curs_pos;
} EditorState;

#endif // !EDITOR_COMMON_H
