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

// ثابت‌های مربوط به منوها
#define MENU "menu"
#define EXIT "exit"
#define MUSIC " MUSIC "
#define SIGN_UP " SIGN UP "
#define LOGIN " LOGIN "
#define BEFORE_GAME_MENU "Before game menu"
#define GUEST_BEFORE_GAME_MENU "Guest before game menu"
#define NEW_GAME " New Game "

char which_menu[50] = MENU;
char* selected_music;

void handle_menu() {
    strcpy(which_menu, display_menu());
}

void handle_music() {
    selected_music = display_music_menu();
    strcpy(which_menu, MENU);
}

void handle_sign_up() {
    user_input_screen();
    strcpy(which_menu, MENU);
}

void handle_login() {
    login_screen();
    // strcpy(which_menu, MENU);
}

void handle_before_game_menu() {
    before_game_menu();
    // strcpy(which_menu, MENU);
}

void handle_guest_before_game_menu() {
    guest_before_game_menu();
    // strcpy(which_menu, MENU);
}

void handle_new_game() {
    if (selected_music != NULL) {
        play_music(selected_music);
    }
    generate_map();
    getch();
    strcpy(which_menu, BEFORE_GAME_MENU);
}

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
        clear();
        refresh();

        if (strcmp(which_menu, EXIT) == 0) {
            display_message();
            break;
        }

        if (strcmp(which_menu, MENU) == 0 || strcmp(which_menu, " Menu ") == 0) {
            handle_menu();
        } else if (strcmp(which_menu, MUSIC) == 0) {
            handle_music();
        } else if (strcmp(which_menu, SIGN_UP) == 0) {
            handle_sign_up();
        } else if (strcmp(which_menu, LOGIN) == 0) {
            handle_login();
        } else if (strcmp(which_menu, BEFORE_GAME_MENU) == 0) {
            handle_before_game_menu();
        } else if (strcmp(which_menu, GUEST_BEFORE_GAME_MENU) == 0) {
            handle_guest_before_game_menu();
        } else if (strcmp(which_menu, NEW_GAME) == 0) {
            handle_new_game();
        } else {
            mvprintw(0, 0, "Invalid option. Returning to menu...");
            strcpy(which_menu, MENU);
        }
    }

    endwin();
    return 0;
}
