#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <ncurses.h>

void get_input(WINDOW *win, int y, int x, char *prompt, char *input, int length);
int username_exists(const char *username);
void save_user_info(const char *username, const char *email, const char *password);

#endif
