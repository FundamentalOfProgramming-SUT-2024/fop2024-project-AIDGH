#include <ncurses.h>
#include <string.h>
#include "menu.h"
#define COLOR_KAKA 5
#define COLOR_GREEN1 30
#define COLOR_YELLOW1 31
#define COLOR_SILVER1 32
#define COLOR_BRONZE1 33
#define COLOR_GOLD1 34
#define COLOR_ORANGE1 35
// #define COLOR_RED 20
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
        int padding = (45 - len) / 2;
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
        " GAME MENU ",
        " LOGIN ",
        " SIGN UP ",
        " SCOREBOARD ",
        " SETTINGS "
    };
    int n_choices = sizeof(choices) / sizeof(char *);

    initscr();
    start_color();
    init_color(COLOR_CYAN, 700, 1000, 1000);
    init_color(COLOR_KAKA, 1000, 600, 600);
    init_color(COLOR_RED, 1000, 0, 0);
    init_color(COLOR_GREEN1, 0, 1000, 0);
    init_color(COLOR_YELLOW1, 1000, 1000, 0);
    init_color(COLOR_SILVER1, 753, 753, 753);
    init_color(COLOR_BRONZE1, 801, 496, 195);
    init_color(COLOR_GOLD1, 1000, 840, 0);
    init_color(COLOR_ORANGE1, 1000, 647, 0);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_KAKA, COLOR_BLACK);
    init_pair(6, COLOR_RED, COLOR_BLACK);
    init_pair(7, COLOR_GREEN1, COLOR_BLACK);
    init_pair(8, COLOR_YELLOW1, COLOR_BLACK);
    init_pair(9, COLOR_BLUE, COLOR_BLACK);
    init_pair(10, COLOR_SILVER1, COLOR_BLACK);
    init_pair(11, COLOR_BRONZE1, COLOR_BLACK);
    init_pair(12, COLOR_GOLD1, COLOR_BLACK);
    init_pair(13, COLOR_WHITE, COLOR_BLACK);
    init_pair(14, COLOR_ORANGE1, COLOR_BLACK);
    curs_set(0);
    clear();
    noecho();
    cbreak();
    int startx, starty, width, height;
    height = 13;
    width = 45;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;
    int y_kaka = 15;
    attron(COLOR_PAIR(4));
    box(stdscr, '|', '-');
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(6));
    mvprintw(LINES / 2 - 24 + y_kaka, (COLS - width) / 2 - 20, "          __     ");
    mvprintw(LINES / 2 - 23 + y_kaka, (COLS - width) / 2 - 20, "      |__|__     ");
    mvprintw(LINES / 2 - 22 + y_kaka, (COLS - width) / 2 - 20, "       __|  |     ");
    attroff(COLOR_PAIR(6));
    attron(COLOR_PAIR(5));
    mvprintw(LINES / 2 - 21 + y_kaka, (COLS - width) / 2 - 20, "      ___|__     ");
    mvprintw(LINES / 2 - 20 + y_kaka, (COLS - width) / 2 - 20, "     /      \\   ");
    mvprintw(LINES / 2 - 19 + y_kaka, (COLS - width) / 2 - 20, "    /  _  _  \\ ");
    mvprintw(LINES / 2 - 18 + y_kaka, (COLS - width) / 2 - 20, "   |          |   ");
    mvprintw(LINES / 2 - 17 + y_kaka, (COLS - width) / 2 - 20, "   |     |    |   ");
    mvprintw(LINES / 2 - 16 + y_kaka, (COLS - width) / 2 - 20, "   |  ______  |   ");
    mvprintw(LINES / 2 - 15 + y_kaka, (COLS - width) / 2 - 20, "  |   |____|  |  ");
    mvprintw(LINES / 2 - 14 + y_kaka, (COLS - width) / 2 - 20, " |            | ");
    mvprintw(LINES / 2 - 13 + y_kaka, (COLS - width) / 2 - 20, " \\           /  ");
    mvprintw(LINES / 2 - 12 + y_kaka, (COLS - width) / 2 - 20, "  \\ ______ /  ");
    attroff(COLOR_PAIR(5));
    attron(COLOR_PAIR(6));
    mvprintw(LINES / 2 - 22 + y_kaka, (COLS - width) / 2 + 45, "       ___      ");
    mvprintw(LINES / 2 - 21 + y_kaka, (COLS - width) / 2 + 45, "      _|*|__     ");
    attroff(COLOR_PAIR(6));
    attron(COLOR_PAIR(5));
    mvprintw(LINES / 2 - 20 + y_kaka, (COLS - width) / 2 + 45, "     /      \\   ");
    mvprintw(LINES / 2 - 19 + y_kaka, (COLS - width) / 2 + 45, "    /  _  _  \\ ");
    mvprintw(LINES / 2 - 18 + y_kaka, (COLS - width) / 2 + 45, "   |  |_||_|  |   ");
    mvprintw(LINES / 2 - 17 + y_kaka, (COLS - width) / 2 + 45, "   |    _|    |   ");
    mvprintw(LINES / 2 - 16 + y_kaka, (COLS - width) / 2 + 45, "   |  ______  |   ");
    mvprintw(LINES / 2 - 15 + y_kaka, (COLS - width) / 2 + 45, "   |    |+|    |  ");
    mvprintw(LINES / 2 - 14 + y_kaka, (COLS - width) / 2 + 45, "   |            | ");
    mvprintw(LINES / 2 - 13 + y_kaka, (COLS - width) / 2 + 45, "    \\           /  ");
    mvprintw(LINES / 2 - 12 + y_kaka, (COLS - width) / 2 + 45, "      \\ ______ /  ");
    attroff(COLOR_PAIR(5));
    menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);

    mvprintw(LINES / 2 - 21, (COLS - width) / 2 + 6, "_____________________________");
    mvprintw(LINES / 2 - 20, (COLS - width) / 2 + 6, "|                           |");
    mvprintw(LINES / 2 - 19, (COLS - width) / 2 + 6, "|  Welcome to AID Rouge =)  |");
    mvprintw(LINES / 2 - 18, (COLS - width) / 2 + 6, "|🗿  🗿  🗿   🗿  🗿  🗿  🗿|");
    mvprintw(LINES / 2 - 17, (COLS - width) / 2 - 14, "____________________|  🗿  🗿  🗿   🗿  🗿  🗿  |______________________");
    mvprintw(LINES / 2 - 16, (COLS - width) / 2 - 14, "|_____________________________________________________________________|");
    mvprintw(LINES / 2 - 15, (COLS - width) / 2 - 14, "|                                                                     |");
    mvprintw(LINES / 2 - 14, (COLS - width) / 2 - 14, "|  Use arrow keys to go up and down, press enter to select a choice!  |");
    mvprintw(LINES / 2 - 13, (COLS - width) / 2 - 14, "|_____________________________________________________________________|");
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
            case 27:
                snprintf(result, sizeof(result), "%s", "exit");
                clrtoeol();
                refresh();
                delwin(menu_win);
                endwin();
                return result;
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
    delwin(menu_win);
    endwin();
    return result;
}
