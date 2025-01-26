#include "user.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>

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
    fscanf(file, "points: %d\n", &user->points);
    fscanf(file, "games count: %d\n", &user->games_count);
    fscanf(file, "play time: %d\n", &user->play_time);
    fscanf(file, "golds: %d\n", &user->golds);
    fscanf(file, "food: %d\n", &user->food);
    fscanf(file, "ancientkey: %d\n", &user->ancientkey);
    fscanf(file, "broken ancientkey: %d\n", &user->brokenancientkey);

    int spellscount, weaponscount;
    char weapons_line[500];
    fscanf(file, "weapons: %d %[^\n]\n", &weaponscount, weapons_line);
    char *weapon = strtok(weapons_line, ", ");
    int i = 0;
    while (weapon != NULL && i < MAX_WEAPONS) {
        strcpy(user->weapons[i], weapon);
        weapon = strtok(NULL, ", ");
        i++;
    }

    char spells_line[500];
    fscanf(file, "spells: %d %[^\n]\n", &spellscount, spells_line);
    char *spell = strtok(spells_line, ", ");
    int j = 0;
    while (spell != NULL && j < MAX_SPELLS) {
        strcpy(user->spells[j], spell);
        spell = strtok(NULL, ", ");
        j++;
    }
    user->spellsCount = spellscount;
    user->weaponsCount = weaponscount;
    fscanf(file, "level: %d\n", &user->level);
    fscanf(file, "map: %[^\n]\n", user->map);

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
