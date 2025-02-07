#ifndef GAP_BUFFER_H_INCLUDED
#define GAP_BUFFER_H_INCLUDED

typedef struct {
  char *start;
  char *end;
  char *buffer;
} GapBuffer;

int str_len(char *str);

GapBuffer *gap_init(char *str);

void move_gap_right(GapBuffer *buffer);

void move_gap_left(GapBuffer *buffer);

void insert_left(GapBuffer *buffer, char item);

#endif
