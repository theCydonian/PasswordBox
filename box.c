#include "box.h"

int main(void) {
    // init screen and sets up screen
    initscr();

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
    move(textWinStarty + 1, textWinStartx + 1);

    // refreshes the screen
    wrefresh(textWin);

    // pause the screen output
    while (getch() != 'q');

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}
