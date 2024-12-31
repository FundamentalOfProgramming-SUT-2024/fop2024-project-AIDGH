#include <ncurses.h>
#include <string.h>
#include "menu.h"

void print_menu(WINDOW *menu_win, int highlight, char **choices, int n_choices) {
    int x, y, i;
    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    wattron(menu_win, COLOR_PAIR(1));
    wborder(menu_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wattroff(menu_win, COLOR_PAIR(1));

    for (i = 0; i < n_choices; ++i) {
        int len = strlen(choices[i]);
        int padding = (40 - len) / 2;
        if (highlight == i + 1) {
            wattron(menu_win, A_BOLD | COLOR_PAIR(2));
            mvwprintw(menu_win, y, padding, "%s", choices[i]);
            wattroff(menu_win, A_BOLD | COLOR_PAIR(2));
        } else {
            wattron(menu_win, COLOR_PAIR(3));
            mvwprintw(menu_win, y, padding, "%s", choices[i]);
            wattroff(menu_win, COLOR_PAIR(3));
        }
        y += 2;
    }
    wrefresh(menu_win);
}

char* display_menu() {
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;
    static char result[50];

    char *choices[] = {
        " LOGIN ",
        " CREATE NEW USER ",
        " SCOREBOARD ",
        " MUSIC "
    };
    int n_choices = sizeof(choices) / sizeof(char *);

    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);

    clear();
    noecho();
    cbreak();
    int startx, starty, width, height;
    height = 12;
    width = 40;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;

    menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);
    mvprintw(13, (COLS - width) / 2 + 9, "Welcome to AID Rouge =)");
    mvprintw(15, (COLS - width) / 2 - 11, "Use arrow keys to go up and down, press enter to select a choice!");
    refresh();
    print_menu(menu_win, highlight, choices, n_choices);
    while(1) {
        c = wgetch(menu_win);
        switch(c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }
        print_menu(menu_win, highlight, choices, n_choices);
        if (choice != 0)
            break;
    }
    snprintf(result, sizeof(result), "%s", choices[choice - 1]);
    clrtoeol();
    refresh();
    endwin();
    return result;
}
