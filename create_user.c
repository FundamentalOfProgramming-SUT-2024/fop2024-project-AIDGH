//Rouge
//By Arad Izadidoost
//403105758
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
// struct user{
//     char *user_name;
//     char *user_email;
//     char *user_password;
// };
// void creat_user()
int main(){
    initscr();
    // struct user new_user;
    // keypad(stdscr, TRUE);
    mvprintw(10, 10, "***********************************\n          * Whats your username?:           *\n          * Whats your email?:              *\n          * Whats your password?:           *\n          *                                 *\n          ***********************************");
    char user[3][1000];
    move(11, 34);
    getstr(user[0]);
    move(12, 31);
    getstr(user[1]);
    move(13, 34);
    noecho();
    getstr(user[2]);
    echo();
    refresh();
    move(15, 0);
    printw("Hi %s\n", user[0]);
    printw("Your email is: %s\n", user[1]);
    printw("Your password is: %s\n", user[2]);
    while(true)
    {
        char c = getch();
        if (c == 'q')
            break;
    }
    refresh();
    endwin();
}