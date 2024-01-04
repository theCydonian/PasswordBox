#include "box.h"

void memory_error(char* input) {
    if (input == NULL) {
        fprintf(stderr, "Malloc Failed.\n");
        exit(1);
    }
}

void memory_error_sodium_free(char* input, char* free_on_fail) {
    if (input == NULL) {
        fprintf(stderr, "Malloc Failed.\n");
        sodium_free(free_on_fail);
        exit(1);
    }
}

int main(void) {
    if (sodium_init() < 0) {
        fprintf(stderr, "Sodium could not be initialized.\n");
        exit(1);
    }
    // init screen and sets up screen
    FILE *f = fopen("/dev/tty", "r+");
    SCREEN *screen = newterm(NULL, f, f);
    set_term(screen);
    //initscr();

    noecho();

    // make window
    int width = 45;
    int height = 15;
    WINDOW *win = newwin(height, width, (LINES-height)/2, (COLS-width)/2);
    refresh();
    box(win, 0, 0);

    // print to screen
    mvwprintw(win, 0, 2, "LUKS Container");
    char innerText[] = "Enter Password:";
    mvwprintw(win, height/2-1, (width - strlen(innerText))/2, "%s", innerText);
    wrefresh(win);

    // text window:
    int textWinHeight = 3;
    int textWinWidth = strlen(innerText) * 2;
    int textWinStarty = (LINES-height)/2 + height/2;
    int textWinStartx = (COLS-width)/2 + (width)/2 - textWinWidth/2;

    WINDOW *textWin = newwin(textWinHeight, textWinWidth, 
            textWinStarty, textWinStartx);
    box(textWin, 0, 0);
    wmove(textWin, 1, 1);

    // refreshes the screen
    wrefresh(textWin);

    // handle typed chars:
    uint64_t size = 8;
    uint64_t length = 0;
    char* output = sodium_malloc(sizeof(char) * size);
    memory_error(output);

    int max = textWinWidth - 3;
    int pos = 0;

    while (true) {
        char newChar = getch(); // TODO: remove int refs

        if (length >= size) {
            size += 8;
            char* new_output = sodium_malloc(sizeof(char) * size);
            memory_error_sodium_free(new_output, output);
            for (uint64_t i = 0; i < size - 8; i++) {
                new_output[i] = output[i];
            }
            sodium_free(output);
            output = new_output;
        }
        if (newChar != '\n') {
            if (newChar == 127) {
                if (length > 0) {
                    length--;
                    pos = (pos - 1) % max;
                }
            } else {
                output[length] = newChar;
                length++;

                pos = pos % max + 1;
            }

            char* pwdText = malloc(sizeof(char) * max);
            memory_error(pwdText);
            for (uint64_t i = 0; i < max; i++) {
                if (i < pos) {
                    pwdText[i] = '*'; // I don't like this char
                } else {
                    pwdText[i] = ' ';
                }
            }
            mvwprintw(textWin, 1, 1, "%s", pwdText);
            wrefresh(textWin);

            wmove(textWin, 1, 1 + pos);
            wrefresh(textWin);

            free(pwdText);
        } else {
            output[length] = '\0';

            char* new_output = sodium_malloc(sizeof(char) * (length + 1));
            memory_error_sodium_free(new_output, output);
            for (uint64_t i = 0; i < length + 1; i++) {
                new_output[i] = output[i];
            }
            sodium_free(output);
            output = new_output;
            break;
        }
    }

    // deallocates memory and ends ncurses
    endwin();
    fflush(stdout);
    printf(output, "%s");
    sodium_free(output);
    fclose(f);
    return 0;
}
