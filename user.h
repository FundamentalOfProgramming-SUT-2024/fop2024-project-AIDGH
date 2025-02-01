#ifndef USER_H
#define USER_H

#define MAX_WEAPONS 10
#define MAX_SPELLS 10
#define MAX_NAME_LENGTH 50

#include "game.h"

typedef struct {
    char username[50];
    char email[50];
    char password[50];
    int points;
    int games_count;
    int play_time;
    int golds;
    int food;
    int health;
    char weapons[MAX_WEAPONS][MAX_NAME_LENGTH];
    int weaponsCount;
    int spellsCount;
    int spellsCount_each[3];
    int weaponsCount_each[5];
    int ancientkey;
    int brokenancientkey;
    char spells[MAX_SPELLS][MAX_NAME_LENGTH];
    int level;
    char map[500];
} User;

int load_user_info(const char *username, User *user);
int save_user_game(Player *user);
int read_all_users(User *users, int max_users);

#endif // USER_H
