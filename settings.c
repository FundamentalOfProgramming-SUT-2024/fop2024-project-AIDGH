#include <ncurses.h>
#include <string.h>
#include "settings.h"

extern char which_menu[50];

char *setting_choices[] = {
    " Difficulty ",
    " Color Change ",
    " Music ",
    " Menu "
};

int n_setting_choices = sizeof(setting_choices) / sizeof(char *);

void print_setting_menu(WINDOW *menu_win, int highlight) {
    int x, y, i;
    x = 5;
    y = 5;
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 0, (50 - strlen(" Setting ")) / 2, "[Setting]");
    mvwprintw(menu_win, 1, 2, "          ____ ____ ____ ____ ____ ");
    mvwprintw(menu_win, 2, 2, " _________|__| |__| |__| |__| |__| ");
    mvwprintw(menu_win, 3, 2, " |        |__| |__| |__| |__| |__| ");
    wattron(menu_win, A_BOLD | COLOR_PAIR(6));
    mvwprintw(menu_win, 14, 2, " |                                  ");
    mvwprintw(menu_win, 4, x - 2, "|");
    mvwprintw(menu_win, 6, x - 2, "|");
    mvwprintw(menu_win, 8, x - 2, "|");
    mvwprintw(menu_win, 10, x - 2, "|");
    mvwprintw(menu_win, 12, x - 2, "|");
    wattroff(menu_win, A_BOLD | COLOR_PAIR(6));
    mvwprintw(menu_win, 12, 4, "         __   __   __   __   __ ");
    mvwprintw(menu_win, 13, 2, " |________|--| |--| |--| |--| |--| ");
    mvwprintw(menu_win, 14, 2, "          |--| |--| |--| |--| |--| ");
    mvwprintw(menu_win, 15, 2, "          \\_/  \\_/  \\_/  \\_/  \\_/");
    
    for (i = 0; i < n_setting_choices; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_BOLD | COLOR_PAIR(6));
            mvwprintw(menu_win, y, x - 2, "* %s", setting_choices[i]);
            wattroff(menu_win, A_BOLD | COLOR_PAIR(6));
        } else {
            mvwprintw(menu_win, y, x - 2, "| %s", setting_choices[i]);
            // mvwprintw(menu_win, y - 1, x - 2, "|");
        }
        y += 2;
    }
    wrefresh(menu_win);
}

void setting_menu() {
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    int startx, starty, width, height;
    height = 18;
    width = 50;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;
    attron(COLOR_PAIR(1));
    box(stdscr, '|', '-');
    attroff(COLOR_PAIR(1));
    menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);
    print_setting_menu(menu_win, highlight);
    
    while (1) {
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_setting_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_setting_choices)
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
        print_setting_menu(menu_win, highlight);
        if (choice != 0)
            break;
    }

    strcpy(which_menu, setting_choices[choice - 1]);
    clrtoeol();
    refresh();
    getch();
    delwin(menu_win);
}