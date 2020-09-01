#include <string.h>
#include <locale.h>
#include <panel.h>
#include <form.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

void drawMainWindows(WINDOW  *mainWindow, WINDOW *statusBar) {

    int maxHeight;
    int maxWidth;

    getmaxyx(stdscr, maxHeight, maxWidth);

    mainWindow = newwin(maxHeight-2, maxWidth, 0, 0);
    box(mainWindow, 0, 0);

    statusBar = newwin(3, maxWidth, maxHeight-3, 0);
    box(statusBar, 0, 0);

    mvwprintw(mainWindow, 1 , 1, "maxHeight = %d, maxWidth = %d", maxHeight, maxWidth);
    mvwprintw(statusBar, 1, 1, "Status Bar Message...");

    wrefresh(stdscr); 
    wrefresh(mainWindow);
    wrefresh(statusBar);

}

void drawQsoEntryForm(WINDOW *qsoFormWindow, FORM *qsoForm, FIELD *field) {

}

int main() {

    setlocale(LC_ALL,"");
    WINDOW *mainWindow;
    WINDOW *statusBar;
    WINDOW *qsoEntryWindow;


    FORM *qsoForm;

    FIELD *field[3];



    int ch;


    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    drawMainWindows(mainWindow, statusBar);

    // Show the QSO Entry Form
    drawQsoEntryForm(qsoFormWindow, qsoForm, field);

    getch();

    delwin(mainWindow);
    delwin(statusBar);
    delwin(qsoFormWindow);
    endwin();
    return 0;
}


void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
    float temp;

    if(win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if(startx != 0)
        x = startx;
    if(starty != 0)
        y = starty;
    if(width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length)/ 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}
