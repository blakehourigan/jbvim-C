#include "../tui/tui.h"
#include "cursor.h"
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static void test_coloring() {
    cursor_set_background(2);
    cursor_set_foreground(5);
    for (int i = 0; i < 150; i++) {
        write(STDOUT_FILENO, "c", 1);
    }
}

static void test_get_coords() {
    Cursor *cursor = malloc(sizeof(Cursor));

    cursor_get_coords(cursor);
    printf("(line: %d, col: %d)", cursor->line, cursor->col);

    free(cursor);
}

int main() {
    struct termios termios_p;
    struct termios termios_og;

    tui_enable_alternate_buffer();
    tui_enable_raw_mode(&termios_p, &termios_og);
    cursor_move_home();

    test_get_coords();

    sleep(5);

    tui_disable_alternate_buffer();
    tui_disable_raw_mode(&termios_og);

    //  b 'R' need to read until big R

    // write(STDOUT_FILENO, "\x1b[6n", 5);
}
