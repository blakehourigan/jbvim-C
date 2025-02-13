#ifndef GAP_BUFFER_H_INCLUDED
#define GAP_BUFFER_H_INCLUDED

typedef struct {
  char *start;
  char *end;
  char *buffer;
  int filled_items;
} GapBuffer;

typedef struct {
  GapBuffer **start;
  GapBuffer **end;
  GapBuffer **buffer;
  int filled_items;
} GapBufferDoc;
int str_len(char *str);

// begin functions for document buffer
GapBufferDoc *gap_init_doc(char *str);

void write_content(GapBufferDoc *doc, int start, int end);

void move_gap_right_doc(GapBufferDoc *doc);

void move_gap_left_doc(GapBufferDoc *doc);

void insert_left_doc(GapBufferDoc *doc, GapBuffer *item);

void grow_buffer_doc(GapBufferDoc *buffer);

void delete_item_doc(GapBufferDoc *doc);

// begin functions for line buffers
GapBuffer *gap_init_line(char *str);

void move_gap_right_line(GapBuffer *buffer);

void move_gap_left_line(GapBuffer *buffer);

void insert_left_line(GapBuffer *buffer, char item);

void delete_item_line(GapBuffer *buffer);

void dbg_buffer(GapBuffer *buffer, int buf_num);

#endif
