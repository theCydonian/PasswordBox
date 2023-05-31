#include "box.h"

void memory_error(char* input) {
    if (input == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(1);
    }
}

int main(void) {
    // init screen and sets up screen
    FILE *f = fopen("/dev/tty", "r+");
    SCREEN *screen = newterm(NULL, f, f);
    set_term(screen);

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
    int size = 8;
    int length = 0;
    char* output = malloc(sizeof(char) * size);
    memory_error(output);

    int max = textWinWidth - 3;
    int pos = 0;

    while (true) {
        char newChar = getch();

        if (length >= size) {
            size += 8;
            output = realloc(output, size * sizeof(char));
            memory_error(output);
        }

        if (newChar != '\n') {
            output[length] = newChar;
            length++;

            pos = pos % max + 1;

            char* pwdText = malloc(sizeof(char) * max);
            memory_error(pwdText);
            for (int i = 0; i < max; i++) {
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
            output = realloc(output, (length) * sizeof(char));
            memory_error(output);
            break;
        }
    }

    // deallocates memory and ends ncurses
    endwin();
    fflush(stdout);
    printf(output, "%s");
    free(output);
    return 0;
}
