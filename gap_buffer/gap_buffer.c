#include "gap_buffer.h"
#include <stdio.h>
#include <stdlib.h>

const int MAX_CAPACITY_INIT = 150;
const int INIT_GAP_END = 149;
const int INIT_GAP_START = 0;

int str_len(char *str) {
  int i = 0;
  while (str[i] != '\0') {
    i++;
  }
  return i;
}

GapBuffer *gap_init(char *str) {
  int desired_size = 150;

  GapBuffer *buffer = malloc(sizeof(GapBuffer) + 150 * sizeof(char));
  buffer->buffer = malloc(150 * sizeof(char));

  buffer->start = &buffer->buffer[0];
  buffer->end = &buffer->buffer[INIT_GAP_END];
  *buffer->end = '\n';

  int len_str = str_len(str);

  for (int i = 0; i < len_str; i++) {
    insert_left(buffer, str[i]);
  }

  return buffer;
};

void move_gap_right(GapBuffer *buffer) {
  if (*buffer->end++ == '\n') {
    return;
  } else {
    char tmp = *buffer->start;
    *buffer->start = *buffer->end++;
    *buffer->end = tmp;

    buffer->start++;
    buffer->end++;
  }
}

void move_gap_left(GapBuffer *buffer) {
  // if this is the first item in the buffer
  if (buffer->start == &buffer->buffer[0]) {
    return;
  } else {
    char tmp = *(buffer->start - 1);
    *buffer->end = tmp;

    buffer->start--;
    buffer->end--;
  }
}

int get_size(GapBuffer *buffer) {
  int i = 0;
  while (buffer->buffer[i] != '\n') {
    i++;
  }
  // include the newline in the len
  i++;
  return i;
}

void grow_buffer(GapBuffer *buffer) {
  int cur_size = get_size(buffer);
  int des_size = cur_size + MAX_CAPACITY_INIT;
  char *new_array =
      malloc((sizeof(char) * cur_size) + (MAX_CAPACITY_INIT * sizeof(char)));

  for (int i = 0; i < cur_size + MAX_CAPACITY_INIT; i++) {
    if (&buffer->buffer[i] < buffer->start) {
      new_array[i] = buffer->buffer[i];
    } else if (buffer->start < &buffer->buffer[i] &&
               &buffer->buffer[i] < buffer->end) {
    } else if (&buffer->buffer[i] > buffer->end) {
      new_array[i] = buffer->buffer[i];
    }
  }
  // free up the old allocation, assign new array to the ptr
  free(buffer->buffer);
  buffer->buffer = new_array;
  buffer->end += MAX_CAPACITY_INIT;
}

void insert_left(GapBuffer *buffer, char item) {
  char *start_next = buffer->start + 1;
  if (start_next == buffer->end) {
    printf("caution, growing");
    grow_buffer(buffer);
  }
  *buffer->start = item;

  buffer->start++;
}

char *get_content(GapBuffer *buffer) {
  int size = get_size(buffer);
  char *str = malloc(sizeof(char) * size);

  for (int i = 0; i < size; i++) {
    str[i] = buffer->buffer[i];
  }
  return str;
}

void dbg_buffer(GapBuffer *buffer) {
  for (int i = 0; i < 150; i++) {
    if (&buffer->buffer[i] == buffer->start) {
      printf("[****(%d: %c)", i, buffer->buffer[i]);
    } else if (&buffer->buffer[i] == buffer->end) {
      printf("(%d: %c)*****]", i, buffer->buffer[i]);
    } else if ((i - 1) % 20 == 0 && i != 1) {
      printf("\n");

    } else {
      printf("(%d: %c)", i, buffer->buffer[i]);
    }
  };
}

// int main() {
//   char str[] = "hello, world!";
//   GapBuffer *buffer = gap_init(str);
//
//   char *content = get_content(buffer);
//   printf("%s", content);
//
//   printf("\n");
//   dbg_buffer(buffer);
//
//   printf("\n");
//
//   for (int i = 0; i < 8; i++) {
//     move_gap_left(buffer);
//     dbg_buffer(buffer);
//     printf("\n");
//     printf("\n");
//   }
//   printf("\n");
//   dbg_buffer(buffer);
//
//   char new_str[] = "bruh moment";
//   int new_str_len = str_len(new_str);
//   for (int i = 0; i < new_str_len; i++) {
//     insert_left(buffer, new_str[i]);
//   }
//
//   printf("\n");
//   printf("\n");
//   dbg_buffer(buffer);
//   content = get_content(buffer);
//
//   printf("%s", content);
//
//   free(content);
//
//   // printf("\n");
//   // for (int i = 0; i < 150; i++) {
//   //   printf("%c", buffer->buffer[i]);
//   // }
//   //     for (int i = 0; i < 150; i++) {
//   //       char c = '0';
//   //       insert_left(buffer, c);
//   //     }
//   //     printf("\n\n\n");
//
//   // size = get_size(buffer);
//
//   // for (int i = 0; i < size; i++) {
//   //   printf("%c", buffer->buffer[i]);
//   // }
// }
