#include <string.h>
#include <locale.h>
#include <panel.h>
#include <form.h>
#include <menu.h>

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

void drawQsoEntryForm(WINDOW *qsoFormWindow, FORM *qsoForm, FIELD **field) {

    int rows, cols;

    /* Initialize the fields */
	field[0] = new_field(1, 10, 6, 1, 0, 0);
	field[1] = new_field(1, 10, 8, 1, 0, 0);
	field[2] = NULL;

    /* Set field options */
    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_AUTOSKIP); /* Don't go to next field when this */
    /* Field is filled up 		*/
    set_field_back(field[1], A_UNDERLINE);
    field_opts_off(field[1], O_AUTOSKIP);

    /* Create the form and post it */
    qsoForm = new_form(field);

    /* Calculate the area required for the form */
    scale_form(qsoForm, &rows, &cols);

    /* Create the window to be associated with the form */
    qsoFormWindow = newwin(rows + 4, cols + 4, 4, 4);
    keypad(qsoFormWindow, TRUE);

    /* Set main window and sub window */
    set_form_win(qsoForm, qsoFormWindow);
    set_form_sub(qsoForm, derwin(qsoFormWindow, rows, cols, 2, 2));

    /* Print a border around the main window and print a title */
    box(qsoFormWindow, 0, 0);
    print_in_middle(qsoFormWindow, 1, 0, cols + 4, "QSO Entry", COLOR_PAIR(1));

    post_form(qsoForm);
    
    wrefresh(qsoFormWindow);

    refresh();
}

// Generic form driver.
// @return -1 on no-go of field_buffer processing, otherwise ready to extract values.
//
int formDriver(int ch, WINDOW *formWindow, FORM *form, FIELD **field) {
    int i;
    int result = 1;

	switch (ch) {
        case 27:
            // Or the current field buffer won't be sync with what is displayed
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_PREV_FIELD);
            pos_form_cursor(form);

            result = -1;
            break;

        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;

        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;

        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;

            // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;

            // Delete the char under the cursor
        case KEY_DC:
            form_driver(form, REQ_DEL_CHAR);
            break;

        default:
            form_driver(form, ch);
            break;
    }

    wrefresh(formWindow);

    return result;

}

int main() {

    setlocale(LC_ALL,"");
    WINDOW *mainWindow;
    WINDOW *statusBar;
    WINDOW *qsoFormWindow;
    FORM *qsoForm;
    FIELD *field[3];

    int ch;
    int formResult;


    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Initialize few color pairs */
   	init_pair(1, COLOR_RED, COLOR_BLACK);


    drawMainWindows(mainWindow, statusBar);

    // Show the QSO Entry Form
    drawQsoEntryForm(qsoFormWindow, qsoForm, field);

    while ((ch = getch()) != KEY_F(1)) {
		formResult = formDriver(ch, qsoFormWindow, qsoForm, field);
    }

    printw("Form result = %d", formResult);
    refresh();

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
