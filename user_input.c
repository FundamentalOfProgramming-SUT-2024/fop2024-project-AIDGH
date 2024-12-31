#include "user_input.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void get_input(WINDOW *win, int y, int x, char *prompt, char *input, int length) {
    char full_prompt[100];
    echo();
    snprintf(full_prompt, sizeof(full_prompt), "%s", prompt); // ساختن رشته فرمتی
    mvwprintw(win, y, x, "%s", full_prompt); // استفاده از رشته فرمتی
    wrefresh(win);
    wgetnstr(win, input, length);
}

int username_exists(const char *username) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        return 0;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, username) != NULL) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void save_user_info(const char *username, const char *email, const char *password) {
    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printw("Error opening file!\n");
    } else {
        fprintf(file, "Username: %s\nEmail: %s\nPassword: %s\n\n", username, email, password);
        fclose(file);
    }
}
