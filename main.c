#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "menu.h"
#include "outro.h"
#include "music.h"
#include "login.h"
#include "sign_up.h"
#include "before_game_menu.h"

char which_menu[50] = "menu";

int main() {
    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    curs_set(0);
    noecho();
    cbreak();

    while (1) {
        if (!strcmp(which_menu, "exit")) {
            clear();
            refresh();
            display_message();
            break;
        } else if (!strcmp(which_menu, "menu")) {
            clear();
            refresh();
            strcpy(which_menu, display_menu());
        } else if (!strcmp(which_menu, " MUSIC ")) {
            clear();
            refresh();
            char* selected_music = display_music_menu();
            if (selected_music != NULL) {
                play_music(selected_music);
            }
            strcpy(which_menu, "menu");
        } else if (!strcmp(which_menu, " SIGN UP ")) {
            clear();
            refresh();
            user_input_screen(); // صدا زدن تابع ثبت نام
            strcpy(which_menu, "menu");
        } else if (!strcmp(which_menu, " LOGIN ")) { // اضافه کردن حالت ورود
            clear();
            refresh();
            login_screen(); // صدا زدن تابع ورود
            strcpy(which_menu, "Before game menu"); // تغییر وضعیت به "Before game menu"
        } else if (!strcmp(which_menu, "Before game menu")) {
            clear();
            refresh();
            before_game_menu(); // صدا زدن تابع منوی قبل از بازی
            strcpy(which_menu, "menu");
        }
    }

    endwin();
    return 0;
}
