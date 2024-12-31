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
            user_input_screen();
            which_menu = "menu";
        }
    }

    endwin();
    return 0;
}
