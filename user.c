#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include "user.h"
#include "map_generator.h"
#include "music.h"
#include "game.h"
#include "before_game_menu.h"
#include "guest_before_game_menu.h"

extern char our_user[50];

int load_user_info(const char *username, User *user) {
    char filepath[100] = "users/";
    strcat(filepath, username);
    strcat(filepath, ".txt");

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        return 0;
    }
    strcpy(user->username, our_user);
    fscanf(file, "health: %d\n", &user->health);
    fscanf(file, "points: %d\n", &user->points);
    fscanf(file, "games count: %d\n", &user->games_count);
    fscanf(file, "play time: %d\n", &user->play_time);
    fscanf(file, "golds: %d\n", &user->golds);
    fscanf(file, "food: %d\n", &user->food);

    int spellscount, weaponscount;
    char weapons_line[500];
    fscanf(file, "weapons: %d %[^\n]\n", &weaponscount, weapons_line);
    char *weapon = strtok(weapons_line, ", ");
    int i = 0;
    while (weapon != NULL && i < 5) {
        strcpy(user->weapons[i], weapon);
        weapon = strtok(NULL, ", ");
        i++;
    }
    for(int i = 0; i < 5; i++){
        int k = strlen(user->weapons[i]) - 1;
        int num = 0;
        int a = 0;
        while(1){
            if(user->weapons[i][k] < '0' || user->weapons[i][k] > '9'){
                break;
            }
            k--;
            a++;
        }
        char num1[4];
        strcpy(num1, user->weapons[i] + k + 1);
        for(int j = 0; j < a; j++){
            num = num * 10 + num1[j] - '0';
        }
        user->weaponsCount_each[i] = num;
    }

    char spells_line[500];
    fscanf(file, "spells: %d %[^\n]\n", &spellscount, spells_line);
    char *spell = strtok(spells_line, ", ");
    int j = 0;
    while (spell != NULL && j < 3) {
        strcpy(user->spells[j], spell);
        spell = strtok(NULL, ", ");
        // sscanf(spell, "%s", user->spells[j]);
        j++;
    }
    fscanf(file, "ancientkey: %d\n", &user->ancientkey);
    fscanf(file, "broken ancientkey: %d\n", &user->brokenancientkey);
    for(int i = 0; i < 3; i++){
        int k = strlen(user->spells[i]) - 1;
        int num = 0;
        int a = 0;
        while(1){
            if(user->spells[i][k] < '0' || user->spells[i][k] > '9'){
                break;
            }
            k--;
            a++;
        }
        char num1[4];
        strcpy(num1, user->spells[i] + k + 1);
        for(int j = 0; j < a; j++){
            num = num * 10 + num1[j] - '0';
        }
        user->spellsCount_each[i] = num;
    }
    user->spellsCount = spellscount;
    user->weaponsCount = weaponscount;
    fscanf(file, "level: %d\n", &user->level);
    fscanf(file, "map: %[^\n]\n", user->map);

    fclose(file);
    return 1;
}

int save_user_game(Player *user) {
    char filepath[100] = "users/";
    strcat(filepath, user->name);
    strcat(filepath, ".txt");

    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        return 0;
    }
    fprintf(file, "health: %d\n", user->health);
    fprintf(file, "points: %d\n", user->points);
    fprintf(file, "games count: %d\n", user->games_count);
    fprintf(file, "play time: %d\n", user->play_time);
    fprintf(file, "golds: %d\n", user->gold);
    fprintf(file, "food: %d\n", user->foodCount);

    fprintf(file, "weapons: %d %s%d, %s%d, %s%d, %s%d, %s%d\n", user->weaponCount,
    user->weapons[0].name, user->weapons[0].count, user->weapons[1].name, user->weapons[1].count, 
    user->weapons[2].name, user->weapons[2].count, user->weapons[3].name, user->weapons[3].count, 
    user->weapons[4].name, user->weapons[4].count);

    fprintf(file, "spells: %d %s%d, %s%d, %s%d\n", user->spellCount, user->spells[0].name, user->spells[0].count,
    user->spells[1].name, user->spells[1].count, user->spells[2].name, user->spells[2].count);
    fprintf(file, "ancientkey: %d\n", user->acientKey);
    fprintf(file, "broken ancientkey: %d\n", user->brokenAcientKey);
    fprintf(file, "level: %d\n", user->level);
    fprintf(file, "map: \n");

    fclose(file);
    return 1;
}
int read_all_users(User *users, int max_users) {
    DIR *dir;
    struct dirent *ent;
    int count = 0;

    if ((dir = opendir("users")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL && count < max_users) {
                char username[50];
                strncpy(username, ent->d_name, strlen(ent->d_name) - 4);
                username[strlen(ent->d_name) - 4] = '\0';
                if (load_user_info(username, &users[count])) {
                    strcpy(users[count].username, username);
                    count++;
                }
            }
        }
        closedir(dir);
    } else {
        perror("Could not open users directory.");
        return 0;
    }
    return count;
}
