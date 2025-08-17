#include "../document_buffer.h"
#include "../line_buffer.h"

#include <string.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    freopen("error_log.txt", "a", stderr);

    size_t MAX_NAME_LENGTH = 150;

    char *doc_name = malloc(MAX_NAME_LENGTH * sizeof(char));

    if (argc > 1) {
        if (strlen(argv[1]) < (MAX_NAME_LENGTH - 1)) {
            strcpy(doc_name, argv[1]);
        } else {
            printf("File name is too long, use a different name.");
            return 1;
        }
    } else {
        printf("Please provide the name of the test file as your first "
               "argument to the program");
        return 1;
    }

    DocumentGapBuffer document = init_document(doc_name);

    // dbg_doc_simple(&document);

    // move_gap_left_doc(&document);
    // move_gap_left_doc(&document);
    // move_gap_left_doc(&document);

    // dbg_doc_simple(&document);

    lineBuffer *current_line = grab_line(&document, 3);

    // dbg_doc_simple(&document);

    insert_line_doc(&document, NULL);
    current_line = grab_next_line(&document, current_line);
    // dbg_doc_simple(&document);

    insert_left_line(current_line, 'c');
    insert_left_line(current_line, 'a');
    insert_left_line(current_line, 'r');
    insert_left_line(current_line, 's');
    insert_left_line(current_line, ' ');
    insert_left_line(current_line, 'g');
    insert_left_line(current_line, 'o');
    insert_left_line(current_line, ' ');

    move_gap_left_doc(&document);

    delete_document_line(&document);

    dbg_doc_simple(&document);
    for (int i = 0; i < 180; i++) {
        insert_line_doc(&document, "hi");
    }
    dbg_doc_simple(&document);

    for (int i = 0; i < 180; i++) {
        delete_document_line(&document);
    }

    dbg_doc_simple(&document);
    insert_line_doc(&document, "HIHIHIHIHI");
    dbg_doc_simple(&document);

    dbg_doc_simple(&document);
    print_content(&document);

    insert_line_doc(&document, "cat in ");
    insert_line_doc(&document, "the hat ");
    insert_line_doc(&document, "drove around ");
    insert_line_doc(&document, "with a bat!");
    dbg_doc_simple(&document);
    print_content(&document);

    move_to_document_start(&document);

    cleanup_document(&document);
}
