#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "menu.h"
#include "outro.h"
#include "music.h"
#include "user_input.h"

int main() {
    char* which_menu = "menu";
    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    curs_set(0);
    noecho();
    cbreak();

    while(1) {
        if(!strcmp(which_menu, "exit")) {
            clear();
            refresh();
            display_message();
            break;
        } else if(!strcmp(which_menu, "menu")) {
            clear();
            refresh();
            which_menu = display_menu();
        } else if (!strcmp(which_menu, " MUSIC ")) {
            clear();
            refresh();
            char* selected_music = display_music_menu();
            if (selected_music != NULL) {
                play_music(selected_music);
            }
            which_menu = "menu";
        } else if (!strcmp(which_menu, " CREATE NEW USER ")) {
            clear();
            refresh();
            int height = 10;
            int width = 40;
            int start_y = (LINES - height) / 2;
            int start_x = (COLS - width) / 2;

            WINDOW *win = newwin(height, width, start_y, start_x);
            box(win, 0, 0);

            char username[50];
            char email[50];
            char password[50];

            // گرفتن اطلاعات از کاربر و بررسی وجود نام کاربری
            while (1) {
                get_input(win, 1, 2, "Enter Username: ", username, 50);
                if (username_exists(username)) {
                    mvwprintw(win, 8, 2, "Username already exists. Try another.");
                } else {
                    break;
                }
                wrefresh(win);
            }
            get_input(win, 3, 2, "Enter Email: ", email, 50);
            get_input(win, 5, 2, "Enter Password: ", password, 50);

            // ذخیره کردن اطلاعات در فایل
            save_user_info(username, email, password);

            mvwprintw(win, 7, 2, "Information saved successfully!");
            wrefresh(win);

            getch();    
            delwin(win); 
            which_menu = "menu";
        }
    }

    endwin();
    return 0;
}
