#include <ncurses.h>
#include <string.h>
#include "user.h"
#include "before_game_menu.h"

extern char which_menu[50];
extern User current_user;

char *choices_before_game[] = {
    " New Game ",
    " Continue Previous Game ",
    " Profile ",
    " Scoreboard ",
    " Settings ",
    " Menu "
};

int n_choices_before_game = sizeof(choices_before_game) / sizeof(char *);

void print_menu_before_game(WINDOW *menu_win, int highlight) {
    int x, y, i;
    x = 5;
    y = 5;
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 0, (50 - strlen(" Before Game Menu ")) / 2, "[Before Game Menu]");
    mvwprintw(menu_win, 1, 2, "          ____ ____ ____ ____ ____ ");
    mvwprintw(menu_win, 2, 2, " _________|__| |__| |__| |__| |__| ");
    mvwprintw(menu_win, 3, 2, " |        |__| |__| |__| |__| |__| ");
    wattron(menu_win, A_BOLD | COLOR_PAIR(6));
    mvwprintw(menu_win, 14, 2, " |                                ");
    mvwprintw(menu_win, 4, x - 2, "|");
    mvwprintw(menu_win, 6, x - 2, "|");
    mvwprintw(menu_win, 8, x - 2, "|");
    mvwprintw(menu_win, 10, x - 2, "|");
    mvwprintw(menu_win, 12, x - 2, "|");
    mvwprintw(menu_win, 14, x - 2, "|");
    mvwprintw(menu_win, 16, x - 2, "|");
    wattroff(menu_win, A_BOLD | COLOR_PAIR(6));
    mvwprintw(menu_win, 16, 4, "         __   __   __   __   __ ");
    mvwprintw(menu_win, 17, 2, " |________|--| |--| |--| |--| |--| ");
    mvwprintw(menu_win, 18, 2, "          |--| |--| |--| |--| |--| ");
    mvwprintw(menu_win, 19, 2, "          \\_/  \\_/  \\_/  \\_/  \\_/");
    
    for (i = 0; i < n_choices_before_game; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_BOLD | COLOR_PAIR(6));
            mvwprintw(menu_win, y, x - 2, "* %s", choices_before_game[i]);
            wattroff(menu_win, A_BOLD | COLOR_PAIR(6));
        } else {
            mvwprintw(menu_win, y, x - 2, "| %s", choices_before_game[i]);
            // mvwprintw(menu_win, y - 1, x - 2, "|");
        }
        y += 2;
    }
    wrefresh(menu_win);
}

void before_game_menu() {
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    int startx, starty, width, height;
    height = 22;
    width = 50;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;
    attron(COLOR_PAIR(1));
    box(stdscr, '|', '-');
    attroff(COLOR_PAIR(1));
    menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);
    print_menu_before_game(menu_win, highlight);
    
    while (1) {
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices_before_game;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices_before_game)
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
        print_menu_before_game(menu_win, highlight);
        if (choice != 0)
            break;
    }

    strcpy(which_menu, choices_before_game[choice - 1]);
    clrtoeol();
    refresh();
    // getch();
    delwin(menu_win);
}