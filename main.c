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
#include "guest_before_game_menu.h"
#include "map_generator.h"
#include "settings.h"
#include "color_change.h"
#include "difficulty.h"

char which_menu[50] = "menu";
char which_user[50] = "guest";
char which_color[50] = " Yellow ";
char which_difficulty[50] = " Easy ";
char* selected_music;
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
        } else if (!strcmp(which_menu, "menu") || !strcmp(which_menu, " Menu ")) {
            clear();
            refresh();
            strcpy(which_menu, display_menu());
        } else if (!strcmp(which_menu, " MUSIC ") || !strcmp(which_menu, " Music ")) {
            clear();
            refresh();
            selected_music = display_music_menu();
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
        } else if (!strcmp(which_menu, "Before game menu")) {
            clear();
            refresh();
            before_game_menu(); // صدا زدن تابع منوی قبل از بازی
        } else if (!strcmp(which_menu, "Guest before game menu")) {
            clear();
            refresh();
            guest_before_game_menu(); // صدا زدن تابع منوی قبل از بازی
        }
        else if (!strcmp(which_menu, " New Game ")) {
            if (selected_music != NULL) {
                play_music(selected_music);
            }
            clear(); 
            refresh(); 
            generate_map();
            getch();
            if(!strcmp(which_user, "guest")){
                strcpy(which_menu, "Guest before game menu");
            }
            else{
                strcpy(which_menu, "Before game menu");
            }
        } else if (!strcmp(which_menu, " Settings ") || !strcmp(which_menu, " SETTINGS ")) {
            clear();
            refresh();
            setting_menu();
        } else if (!strcmp(which_menu, " Color Change ")) {
            clear();
            refresh();
            color_change();
            strcpy(which_menu, " Settings ");
        } else if (!strcmp(which_menu, " Difficulty ")) {
            clear();
            refresh();
            difficulty_change();
            strcpy(which_menu, " Settings ");
        }
    }

    endwin();
    return 0;
}
