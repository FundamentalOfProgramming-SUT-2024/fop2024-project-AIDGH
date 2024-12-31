//Rouge
//By Arad Izadidoost
//403105758
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
int main(){
    char *user_choice; 
    user_choice = display_menu();
    printf("You choosed: %s\n", user_choice);
}
