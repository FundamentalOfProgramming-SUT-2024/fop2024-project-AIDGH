#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "user.h"
#include "map_generator.h"
#include "music.h"
#include "game.h"
#include "before_game_menu.h"
#include "guest_before_game_menu.h"
#include "pause_menu.h"

// Room rooms[12];
extern Mix_Music *music;
Mix_Music *effects;
Level *levels[6];
Game *game;
// Map whole_map[6][200][200];
Corridor *corridors[50];
Player *player;
extern User current_user;
extern int total_rooms;
int current_level;
int unlocked_level;
char which_pause_menu[50];
extern char which_color[50];
extern int new_game_check;
int color_pair_num;
int escape_check = 0;
int difficulty_efficiency;
char diff_from_map = 0;
int in_emoji = 0;
int lock_color = 6;
int password_disapear = 0;
int last_attemps = 0;
int back_front = 0;
int in_nightmare = 0;
int in_spell = 0;
int all_print = 0;
int t_check = 0;
int g_check = 0;
int f_check = 0;
int x_check = 0;
int m_check = 0;
int count_move = 1;
int poisonous_food_check = 0;
int last_item = '.';
int move_or_not = 0;
int is_enemy_or_not = 0;
int is_enemy_or_not2 = 0;
int move_click;
int direction;
int is_last_room = 0;
int is_pass_lock = 0;
extern char* selected_music;
int x_save, y_save, level_save, is_battle = 0, after_battle = 0;
time_t start_time;
time_t quote_time;
time_t quote_time2;
time_t move_time;
time_t move_time_2;
time_t move_time_lock;
time_t rotten_time;
time_t rotten_time_check;

void in_emoji_check(){
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '*'){
        in_emoji = 1;
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'E'){
        in_emoji = 1;
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'P'){
        in_emoji = 1;
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'H'){
        in_emoji = 1;
        return;
    }
    // if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '1'){
    //     in_emoji = 1;
    //     return;
    // }
    // if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '2'){
    //     in_emoji = 1;
    //     return;
    // }
    // if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '3'){
    //     in_emoji = 1;
    //     return;
    // } 
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '6'){
        in_emoji = 1;
        return;
    } 
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '7'){
        in_emoji = 1;
        return;
    } 
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '8'){
        in_emoji = 1;
        return;
    } 
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '9'){
        in_emoji = 1;
        return;
    } 
    // if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '!'){
    //     in_emoji = 1;
    //     return;
    // } 
    // if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '4'){
    //     in_emoji = 1;
    //     return;
    // }
    // if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '5'){
    //     in_emoji = 1;
    //     return;
    // }
}
int is_enemy(int y, int x){
    if(game->whole_map[game->currentLevel][y][x].item == 'D' 
        || game->whole_map[game->currentLevel][y][x].item == 'F'
        || game->whole_map[game->currentLevel][y][x].item == 'G'
        || game->whole_map[game->currentLevel][y][x].item == 'S'
        || game->whole_map[game->currentLevel][y][x].item == 'U'
        || game->whole_map[game->currentLevel][y][x].item == 'I'
        || game->whole_map[game->currentLevel][y][x].item == 'i' - 32){
            return 1;
        }
    return 0;
}
void draw_room(WINDOW *game_win, Room *room, int room_number) {
    // wattron(game_win, COLOR_PAIR(color_pair_num));
    if(!m_check){
        wattroff(game_win, COLOR_PAIR(3));
    }
    if(room->type == 's' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(15));
    }
    else if(room->type == 't' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(22));
    }
    else if(room->type == 'b' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(14));
    }
    else if(room->type == 'p' && room->isVisible == true){
        if(room_number == game->player->room_num){
            in_spell = 1;
        }
        wattron(game_win, COLOR_PAIR(17));
    }
    if(room->type == 'm' && room->isVisible == true && all_print == 0){
        in_nightmare = 1;
        return;
    }
    if(room->type == 'm' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(6));
    }
    all_print = 1;
    in_nightmare = 0;
    room->lock_attemps = 0;
    // if(room->type == 'b'){
    //     for(int i = 0; i < room->pillarCount; i++){
    //         wattron(game_win, COLOR_PAIR(16));
    //         mvwprintw(game_win, room->pillars[i].y, room->pillars[i].x, "▒");
    //         wattroff(game_win, COLOR_PAIR(16));
    //     }
    //     wattron(game_win, COLOR_PAIR(14));
    //     for (int y = room->y; y < room->y + room->height; y++) {
    //         for (int x = room->x; x < room->x + room->width; x++) {
    //             if(y == room->y && x == room->x){
    //                 mvwprintw(game_win, y, x, "┌");
    //             }
    //             else if(y == room->y + room->height - 1 && x == room->x){
    //                 mvwprintw(game_win, y, x, "└");
    //             }
    //             else if(y == room->y && x == room->x + room->width - 1){
    //                 mvwprintw(game_win, y, x, "┐");
    //             }
    //             else if(y == room->y + room->height - 1 && x == room->x + room->width - 1){
    //                 mvwprintw(game_win, y, x, "┘");
    //             }
    //             else if (y == room->y) {
    //                 mvwprintw(game_win, y, x, "─");
    //             } else if (y == room->y + room->height - 1) {
    //                 mvwprintw(game_win, y, x, "─");
    //             } else if (x == room->x || x == room->x + room->width - 1) {
    //                 mvwprintw(game_win, y, x, "│");
    //             } else {
    //                 mvwprintw(game_win, y, x, ".");
    //             }
    //         }
    //     }
    //     wattroff(game_win, COLOR_PAIR(14));
    //     wattron(game_win, COLOR_PAIR(6));
    //     for(int i = 0; i < room->enemyCount; i++){
    //         if(room->enemies[i].isVisible == true){
    //             mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x, "%c", room->enemies[i].type - 32);
    //         }
    //     }
    //     wattroff(game_win, COLOR_PAIR(6));
    //     wattron(game_win, COLOR_PAIR(color_pair_num));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "@");
    //     wattroff(game_win, COLOR_PAIR(color_pair_num));
    //     return;
    // }
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            if(y == room->y && x == room->x){
                mvwprintw(game_win, y, x, "┌");
            }
            else if(y == room->y + room->height - 1 && x == room->x){
                mvwprintw(game_win, y, x, "└");
            }
            else if(y == room->y && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┐");
            }
            else if(y == room->y + room->height - 1 && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┘");
            }
            else if (y == room->y) {
                mvwprintw(game_win, y, x, "─");
            } else if (y == room->y + room->height - 1) {
                mvwprintw(game_win, y, x, "─");
            } else if (x == room->x || x == room->x + room->width - 1) {
                mvwprintw(game_win, y, x, "│");
            } else {
                mvwprintw(game_win, y, x, ".");
            }
        }
    }
    for(int i = 0; i < room->pillarCount; i++){
        wattron(game_win, COLOR_PAIR(16));
        if(room->type == 'b'){
            wattron(game_win, COLOR_PAIR(20));
        }
        mvwprintw(game_win, room->pillars[i].y, room->pillars[i].x, "▒");
        if(room->type == 'b'){
            wattroff(game_win, COLOR_PAIR(20));
        }
        wattroff(game_win, COLOR_PAIR(16));
    }
    for(int i = 0; i < room->foodCount; i++){
        if(room->foods[i].isUsed == false){
            wattron(game_win, COLOR_PAIR(7));
            mvwprintw(game_win, room->foods[i].cord.y, room->foods[i].cord.x, "♣");
            wattroff(game_win, COLOR_PAIR(7));
        }
    }
    for(int i = 0; i < room->shieldCount; i++){
        if(room->shields[i].isUsed == false){
            wattron(game_win, COLOR_PAIR(19));
            mvwprintw(game_win, room->shields[i].cord.y, room->shields[i].cord.x + 1, " ");
            mvwprintw(game_win, room->shields[i].cord.y, room->shields[i].cord.x, "🛡");
            wattroff(game_win, COLOR_PAIR(19));
        }
    }
    for(int i = 0; i < room->goldCount; i++){
        if(room->golds[i].isUsed == false){
            if(room->golds[i].type == 'n'){
                wattron(game_win, COLOR_PAIR(12));
                mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "¤");
                wattroff(game_win, COLOR_PAIR(12));
            }
            else if(room->golds[i].type == 'b'){
                mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "🌑");
            }
        }
    }
    for(int i = 0; i < room->spellCount; i++){
        if(room->spells[i].isUsed == false){
            if(room->spells[i].type == 's'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍾");
            }
            else if(room->spells[i].type == 'p'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍷");
            }
            else if(room->spells[i].type == 'h'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍸");
            }
        }
    }
    for(int i = 0; i < room->specialfoodCount; i++){
        if(room->specialfoods[i].isUsed == false){
            if(room->specialfoods[i].type == 'n' || room->specialfoods[i].type == 'r'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍓");
            }
            else if(room->specialfoods[i].type == 'p'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍘");
            }
            else if(room->specialfoods[i].type == 's'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍟");
            }
        }
    }
    for(int i = 0; i < room->weaponCount; i++){
        if(room->weapons[i].isUsed == false){
            if(room->weapons[i].type == 'm'){
                wattron(game_win, COLOR_PAIR(16));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U00002692');
                wattroff(game_win, COLOR_PAIR(16));
            }
            else if(room->weapons[i].type == 'd'){
                wattron(game_win, COLOR_PAIR(10));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U0001F5E1');
                wattroff(game_win, COLOR_PAIR(10));
            }
            else if(room->weapons[i].type == 'w'){
                wattron(game_win, COLOR_PAIR(17));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "⚚");
                wattroff(game_win, COLOR_PAIR(17));
                //  L'\U0001FA84'
            }
            else if(room->weapons[i].type == 'a'){
                wattron(game_win, COLOR_PAIR(18));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U000027B3');
                wattroff(game_win, COLOR_PAIR(18));
            }
            else if(room->weapons[i].type == 's'){
                wattron(game_win, COLOR_PAIR(8));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U00002694');
                wattroff(game_win, COLOR_PAIR(8));
            }
        }
    }
    for(int i = 0; i < room->trapCount; i++){
        if(room->traps[i].isVisible == true || t_check){
            wattron(game_win, COLOR_PAIR(6));
            mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, "^");
            wattroff(game_win, COLOR_PAIR(6));
        }
    }
    for(int i = 0; i < room->doorCount; i++){
        if(room->doors[i].type == 'n'){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "+");
        }
        else if(room->doors[i].type == 'l' || room->doors[i].type == 'h'){
            if(room->doors[i].is_open == false){
                wattron(game_win, COLOR_PAIR(6));
            }
            else{
                wattron(game_win, COLOR_PAIR(7));
            }
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "☯");
            if(room->doors[i].is_open == false){
                wattroff(game_win, COLOR_PAIR(6));
            }
            else{
                wattroff(game_win, COLOR_PAIR(7));
            }
            wattron(game_win, COLOR_PAIR(color_pair_num));
            mvwprintw(game_win, room->password_generator.y, room->password_generator.x, "&");
            wattroff(game_win, COLOR_PAIR(color_pair_num));
            if(room->doors[i].type == 'h'){
                wattron(game_win, COLOR_PAIR(color_pair_num));
                mvwprintw(game_win, room->password_generator2.y, room->password_generator2.x, "&");
                wattroff(game_win, COLOR_PAIR(color_pair_num));
            }
        }
        else if((room->doors[i].type == 's' && room->doors[i].password == 1) || t_check){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "‽");
        }
    }
    for(int i = 0; i < room->enemyCount; i++){
        if(room->enemies[i].isVisible == true){
            wattron(game_win, COLOR_PAIR(14));
            if((room->type == 't' && room->isVisible == true) || room->type == 'b'){
                wattron(game_win, COLOR_PAIR(6));
            }
            mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x, "%c", room->enemies[i].type - 32);
            if((room->type == 't' && room->isVisible == true) || room->type == 'b'){
                wattroff(game_win, COLOR_PAIR(6));
            }
            wattroff(game_win, COLOR_PAIR(14));
        }
    }
    if(room->window.x != 0 && room->window.x != 0){
        mvwprintw(game_win, room->window.y, room->window.x, "=");
    }
    if(room_number == 0 && game->currentLevel != 0){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, "<");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    else if(room->stairCount == 1){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, ">");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    if(room->type == 's' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(15));
    }
    else if((room->type == 't' && room->isVisible == true) || room->type == 'b'){
        wattroff(game_win, COLOR_PAIR(22));
        in_emoji_check();
        if(in_emoji == 0){
            wattron(game_win, COLOR_PAIR(color_pair_num));
            mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "@");
            wattroff(game_win, COLOR_PAIR(color_pair_num));
        }
    }
    else if(room->type == 'p' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(17));
    }
    else if(room->type == 'm' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(6));
    }
    if(room->keyCount == 1){
    
        wattron(game_win, COLOR_PAIR(12));
        mvwprintw(game_win, room->ancientkey.y, room->ancientkey.x, "Δ");
        wattroff(game_win, COLOR_PAIR(12));
    }
    // wattroff(game_win, COLOR_PAIR(color_pair_num));
}
void draw_last_room(WINDOW *game_win, Room *room, int room_number) {
    // wattron(game_win, COLOR_PAIR(color_pair_num));
    wattroff(game_win, COLOR_PAIR(3));
    wattron(game_win, COLOR_PAIR(14));
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            if(y == room->y && x == room->x){
                mvwprintw(game_win, y, x, "┌");
            }
            else if(y == room->y + room->height - 1 && x == room->x){
                mvwprintw(game_win, y, x, "└");
            }
            else if(y == room->y && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┐");
            }
            else if(y == room->y + room->height - 1 && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┘");
            }
            else if (y == room->y) {
                mvwprintw(game_win, y, x, "─");
            } else if (y == room->y + room->height - 1) {
                mvwprintw(game_win, y, x, "─");
            } else if (x == room->x || x == room->x + room->width - 1) {
                mvwprintw(game_win, y, x, "│");
            } else {
                game->whole_map[game->currentLevel][y][x].item = '.';
                mvwprintw(game_win, y, x, ".");
            }
        }
    }
    for(int i = 0; i < room->trapCount; i++){
        wattron(game_win, COLOR_PAIR(6));
        mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, "^");
        wattroff(game_win, COLOR_PAIR(6));
        game->whole_map[game->currentLevel][room->traps[i].cord.y][room->traps[i].cord.x].item = '^';
    }
    for(int i = 0; i < room->doorCount; i++){
        if(room->doors[i].type == 'n'){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "+");
        }
        else if(room->doors[i].type == 'l' || room->doors[i].type == 'h'){
            if(room->doors[i].is_open == false){
                wattron(game_win, COLOR_PAIR(6));
            }
            else{
                wattron(game_win, COLOR_PAIR(7));
            }
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "☯");
            if(room->doors[i].is_open == false){
                wattroff(game_win, COLOR_PAIR(6));
            }
            else{
                wattroff(game_win, COLOR_PAIR(7));
            }
            wattron(game_win, COLOR_PAIR(color_pair_num));
            mvwprintw(game_win, room->password_generator.y, room->password_generator.x, "&");
            wattroff(game_win, COLOR_PAIR(color_pair_num));
        }
        else if((room->doors[i].type == 's' && room->doors[i].password == 1) || t_check){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "‽");
        }
    }
    for(int i = 0; i < room->enemyCount; i++){
        // if(room->enemies[i].type == 'I'){
        //     game->whole_map[game->currentLevel][room->enemies[i].cord.y][room->enemies[i].cord.x].item = 'I';
        //     wattron(game_win, COLOR_PAIR(14));
        //     if(room->type == 't' && room->isVisible == true){
        //         wattron(game_win, COLOR_PAIR(6));
        //     }
        //     mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x, "%c", room->enemies[i].type - 32);
        //     if(room->type == 't' && room->isVisible == true){
        //         wattroff(game_win, COLOR_PAIR(6));
        //     }
        //     wattroff(game_win, COLOR_PAIR(14));
        // }
        if(room->enemies[i].isAlive == true){
            wattron(game_win, COLOR_PAIR(17));
            game->whole_map[game->currentLevel][room->enemies[i].cord.y][room->enemies[i].cord.x].item = 'I';
            mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x, "%c", room->enemies[i].type - 32);
            if(room->enemies[i].health > 100){
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x, "─");
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x, "─");
                mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x + 1, "│");
                mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x - 1, "│");
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x + 1, "╯");
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x - 1, "╰");
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x + 1, "╮");
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x - 1, "╭");
            }
            else if(room->enemies[i].health > 50){
                wattron(game_win, COLOR_PAIR(6));
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x, "─");
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x, "─");
                mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x + 1, "│");
                mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x - 1, "│");
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x + 1, "╯");
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x - 1, "╰");
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x + 1, "╮");
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x - 1, "╭");
            }
            else if(room->enemies[i].health <= 50){
                wattron(game_win, COLOR_PAIR(21));
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x, "─");
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x, "─");
                mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x + 1, "│");
                mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x - 1, "│");
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x + 1, "╯");
                mvwprintw(game_win, room->enemies[i].cord.y + 1, room->enemies[i].cord.x - 1, "╰");
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x + 1, "╮");
                mvwprintw(game_win, room->enemies[i].cord.y - 1, room->enemies[i].cord.x - 1, "╭");
            }
            wattroff(game_win, COLOR_PAIR(17));
            wattroff(game_win, COLOR_PAIR(21));
            wattroff(game_win, COLOR_PAIR(6));
        }
    }
    if(room->window.x != 0 && room->window.x != 0){
        mvwprintw(game_win, room->window.y, room->window.x, "=");
    }
    if(room_number == 0 && game->currentLevel != 0){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, "<");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    else if(room->stairCount == 1){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, ">");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    else if(room->type == 't' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(12));
    }
    if(room->keyCount == 1){
        wattron(game_win, COLOR_PAIR(12));
        game->whole_map[game->currentLevel][room->ancientkey.y][room->ancientkey.x].item = 'A';
        mvwprintw(game_win, room->ancientkey.y, room->ancientkey.x, "Δ");
        wattroff(game_win, COLOR_PAIR(12));
    }
    // for (int y = room->y; y < room->y + room->height; y++) {
    //     for (int x = room->x; x < room->x + room->width; x++) {
    //         mvwprintw(game_win, y, x, "%c", game->whole_map[game->currentLevel][y][x].item);
    //     }
    // }
    wattron(game_win, COLOR_PAIR(color_pair_num));
    mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "@");
    wattroff(game_win, COLOR_PAIR(color_pair_num));
    // wattroff(game_win, COLOR_PAIR(color_pair_num));
}
void draw_nightmare_room(WINDOW *game_win, Room *room, int room_number) {
    if(room->type == 'm' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(6));
        in_nightmare = 1;
    }
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            if(y == room->y && x == room->x){
                mvwprintw(game_win, y, x, "┌");
            }
            else if(y == room->y + room->height - 1 && x == room->x){
                mvwprintw(game_win, y, x, "└");
            }
            else if(y == room->y && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┐");
            }
            else if(y == room->y + room->height - 1 && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┘");
            }
            else if (y == room->y) {
                mvwprintw(game_win, y, x, "─");
            } else if (y == room->y + room->height - 1) {
                mvwprintw(game_win, y, x, "─");
            } else if (x == room->x || x == room->x + room->width - 1) {
                mvwprintw(game_win, y, x, "│");
            } else {
                if((x - game->player->cord.x <= 2 && game->player->cord.x - x <= 2) && (y - game->player->cord.y <= 2 && game->player->cord.y - y <= 2)){
                    mvwprintw(game_win, y, x, ".");
                }
                else{
                    mvwprintw(game_win, y, x, " ");
                }
            }
        }
    }
    for(int i = 0; i < room->pillarCount; i++){
        wattron(game_win, COLOR_PAIR(16));
        if((room->pillars[i].x - game->player->cord.x <= 2 && game->player->cord.x - room->pillars[i].x <= 2)
        && (room->pillars[i].y - game->player->cord.y <= 2 && game->player->cord.y - room->pillars[i].y <= 2)){
            mvwprintw(game_win, room->pillars[i].y, room->pillars[i].x, "▒");
        }
        else{
            mvwprintw(game_win, room->pillars[i].y, room->pillars[i].x, " ");
        }
        wattroff(game_win, COLOR_PAIR(16));
    }
    for(int i = 0; i < room->foodCount; i++){
        if(room->foods[i].isUsed == false){
            wattron(game_win, COLOR_PAIR(7));
            if((room->foods[i].cord.x - game->player->cord.x <= 2 && game->player->cord.x - room->foods[i].cord.x<= 2)
            && (room->foods[i].cord.y - game->player->cord.y <= 2 && game->player->cord.y - room->foods[i].cord.y <= 2)){
                mvwprintw(game_win, room->foods[i].cord.y, room->foods[i].cord.x, "♣");
            }
            else{
                mvwprintw(game_win, room->foods[i].cord.y, room->foods[i].cord.x, " ");
            }
            wattroff(game_win, COLOR_PAIR(7));
        }
    }
    for(int i = 0; i < room->shieldCount; i++){
        if(room->shields[i].isUsed == false){
            wattron(game_win, COLOR_PAIR(19));
            if((room->shields[i].cord.x - game->player->cord.x <= 2 && game->player->cord.x - room->shields[i].cord.x<= 2)
            && (room->shields[i].cord.y - game->player->cord.y <= 2 && game->player->cord.y - room->shields[i].cord.y <= 2)){
                mvwprintw(game_win, room->shields[i].cord.y, room->shields[i].cord.x, "🛡");
            }
            else{
                mvwprintw(game_win, room->shields[i].cord.y, room->shields[i].cord.x, " ");
            }
            wattroff(game_win, COLOR_PAIR(19));
        }
    }
    for(int i = 0; i < room->goldCount; i++){
        if(room->golds[i].isUsed == false){
            if((room->golds[i].cord.x - game->player->cord.x <= 2 && game->player->cord.x - room->golds[i].cord.x <= 2)
            && (room->golds[i].cord.y - game->player->cord.y <= 2 && game->player->cord.y - room->golds[i].cord.y <= 2)){
                if(room->golds[i].type == 'n'){
                    wattron(game_win, COLOR_PAIR(12));
                    mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "¤");
                    wattroff(game_win, COLOR_PAIR(12));
                }
                else if(room->golds[i].type == 'b'){
                    mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "🌑");
                }
            }
            else{
                mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "  ");
            }
        }
    }
    for(int i = 0; i < room->spellCount; i++){
        if(room->spells[i].isUsed == false){
            if((room->spells[i].cord.x - game->player->cord.x <= 2 && game->player->cord.x - room->spells[i].cord.x <= 2)
            && (room->spells[i].cord.y - game->player->cord.y <= 2 && game->player->cord.y - room->spells[i].cord.y <= 2)){
                if(room->spells[i].type == 's'){
                    mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍾");
                }
                else if(room->spells[i].type == 'p'){
                    mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍷");
                }
                else if(room->spells[i].type == 'h'){
                    mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍸");
                }
            }
            else{
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, " ");
            }
        }
    }
    for(int i = 0; i < room->specialfoodCount; i++){
        if(room->specialfoods[i].isUsed == false){
            if((room->specialfoods[i].cord.x - game->player->cord.x <= 2 && game->player->cord.x - room->specialfoods[i].cord.x <= 2)
            && (room->specialfoods[i].cord.y - game->player->cord.y <= 2 && game->player->cord.y - room->specialfoods[i].cord.y <= 2)){
                if(room->specialfoods[i].type == 'n' || room->specialfoods[i].type == 'r'){
                    mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍓");
                }
                else if(room->specialfoods[i].type == 'p'){
                    mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍘");
                }
                else if(room->specialfoods[i].type == 's'){
                    mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍟");
                }
            }
            else{
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, " ");
            }
        }
    }
    for(int i = 0; i < room->weaponCount; i++){
        if(room->weapons[i].isUsed == false){
            if((room->weapons[i].cord.x - game->player->cord.x <= 2 && game->player->cord.x - room->weapons[i].cord.x <= 2)
            && (room->weapons[i].cord.y - game->player->cord.y <= 2 && game->player->cord.y - room->weapons[i].cord.y <= 2)){
                if(room->weapons[i].type == 'm'){
                    wattron(game_win, COLOR_PAIR(16));
                    mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U00002692');
                    wattroff(game_win, COLOR_PAIR(16));
                }
                else if(room->weapons[i].type == 'd'){
                    wattron(game_win, COLOR_PAIR(10));
                    mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U0001F5E1');
                    wattroff(game_win, COLOR_PAIR(10));
                }
                else if(room->weapons[i].type == 'w'){
                    wattron(game_win, COLOR_PAIR(17));
                    mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "⚚");
                    wattroff(game_win, COLOR_PAIR(17));
                    //  L'\U0001FA84'
                }
                else if(room->weapons[i].type == 'a'){
                    wattron(game_win, COLOR_PAIR(18));
                    mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U000027B3');
                    wattroff(game_win, COLOR_PAIR(18));
                }
                else if(room->weapons[i].type == 's'){
                    wattron(game_win, COLOR_PAIR(8));
                    mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U00002694');
                    wattroff(game_win, COLOR_PAIR(8));
                }
            }
            else{
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, " ");
            }
        }
    }
    for(int i = 0; i < room->trapCount; i++){
        if(room->traps[i].isVisible == true){
            wattron(game_win, COLOR_PAIR(6));
            if((room->traps[i].cord.x - game->player->cord.x <= 2 && game->player->cord.x - room->traps[i].cord.x <= 2)
            && (room->traps[i].cord.y - game->player->cord.y <= 2 && game->player->cord.y - room->traps[i].cord.y <= 2)){
                mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, "^");
            }
            else{
                mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, " ");
            }
            wattroff(game_win, COLOR_PAIR(6));
        }
    }
    for(int i = 0; i < room->doorCount; i++){
        if(room->doors[i].type == 'n'){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "+");
        }
        else if(room->doors[i].type == 's' && room->doors[i].password == 1){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "‽");
        }
        else if(room->doors[i].type == 'l' || room->doors[i].type == 'h'){
            if(room->doors[i].is_open == false){
                wattron(game_win, COLOR_PAIR(6));
            }
            else{
                wattron(game_win, COLOR_PAIR(7));
            }
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "☯");
            if(room->doors[i].is_open == false){
                wattroff(game_win, COLOR_PAIR(6));
            }
            else{
                wattroff(game_win, COLOR_PAIR(7));
            }
            wattron(game_win, COLOR_PAIR(color_pair_num));
            if((room->password_generator.x - game->player->cord.x <= 2 && game->player->cord.x - room->password_generator.x <= 2)
            && (room->password_generator.y - game->player->cord.y <= 2 && game->player->cord.y - room->password_generator.y <= 2)){
                mvwprintw(game_win, room->password_generator.y, room->password_generator.x, "&");
            }
            else{
                mvwprintw(game_win, room->password_generator.y, room->password_generator.x, " ");
            }
            if(room->doors[i].type == 'h'){
            if((room->password_generator2.x - game->player->cord.x <= 2 && game->player->cord.x - room->password_generator2.x <= 2)
                && (room->password_generator2.y - game->player->cord.y <= 2 && game->player->cord.y - room->password_generator2.y <= 2)){
                    mvwprintw(game_win, room->password_generator2.y, room->password_generator2.x, "&");
                }
                else{
                    mvwprintw(game_win, room->password_generator2.y, room->password_generator2.x, " ");
                }
            }
            wattroff(game_win, COLOR_PAIR(color_pair_num));
        }
    }
    if(room->window.x != 0 && room->window.x != 0){
        mvwprintw(game_win, room->window.y, room->window.x, "=");
    }
    if(room_number == 0 && game->currentLevel != 0){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, "<");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    else if(room->stairCount == 1){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, ">");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    else if(room->type == 'm' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(6));
    }
    if(room->keyCount == 1){
        wattron(game_win, COLOR_PAIR(12));
        if((room->ancientkey.x - game->player->cord.x <= 2 && game->player->cord.x - room->ancientkey.x <= 2)
        && (room->ancientkey.y - game->player->cord.y <= 2 && game->player->cord.y - room->ancientkey.y <= 2)){
            mvwprintw(game_win, room->ancientkey.y, room->ancientkey.x, "Δ");
        }
        else{
            mvwprintw(game_win, room->ancientkey.y, room->ancientkey.x, " ");
        }
        wattroff(game_win, COLOR_PAIR(12));
    }
}
void draw_attacked_room(WINDOW *game_win, Room *room, int room_number) {
    if(is_last_room){
        draw_last_room(game_win, room, room_number);
        return;
    }
    int temp = color_pair_num;
    if(color_pair_num == 0){
        color_pair_num = 14;
    }
    if(room->type == 'm' || room->type == 's' || room->type == 'p'){
    
        return;
    }
    else if(room->type == 't' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(12));
    }
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            if(y == room->y && x == room->x){
                mvwprintw(game_win, y, x, "┌");
            }
            else if(y == room->y + room->height - 1 && x == room->x){
                mvwprintw(game_win, y, x, "└");
            }
            else if(y == room->y && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┐");
            }
            else if(y == room->y + room->height - 1 && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┘");
            }
            else if (y == room->y) {
                mvwprintw(game_win, y, x, "─");
            } else if (y == room->y + room->height - 1) {
                mvwprintw(game_win, y, x, "─");
            } else if (x == room->x || x == room->x + room->width - 1) {
                mvwprintw(game_win, y, x, "│");
            } else {
                if((x - game->player->cord.x <= 1 && game->player->cord.x - x <= 1) && (y - game->player->cord.y <= 1 && game->player->cord.y - y <= 1)){
                    wattron(game_win, COLOR_PAIR(color_pair_num));
                    mvwprintw(game_win, y, x, ".");
                    wattroff(game_win, COLOR_PAIR(color_pair_num));
                }
                else{
                    if(room->type == 't'){
                        wattron(game_win, COLOR_PAIR(12));
                    }
                    mvwprintw(game_win, y, x, ".");
                    if(room->type == 't'){
                        wattroff(game_win, COLOR_PAIR(12));
                    }
                }
            }
        }
    }
    for(int i = 0; i < room->pillarCount; i++){
        wattron(game_win, COLOR_PAIR(16));
        mvwprintw(game_win, room->pillars[i].y, room->pillars[i].x, "▒");
        wattroff(game_win, COLOR_PAIR(16));
    }
    for(int i = 0; i < room->foodCount; i++){
        if(room->foods[i].isUsed == false){
            wattron(game_win, COLOR_PAIR(7));
            if((room->foods[i].cord.x - game->player->cord.x <= 1 && game->player->cord.x - room->foods[i].cord.x<= 1)
            && (room->foods[i].cord.y - game->player->cord.y <= 1 && game->player->cord.y - room->foods[i].cord.y <= 1)){
                // wattron(game_win, COLOR_PAIR(color_pair_num));
                wattron(game_win, COLOR_PAIR(7));
                mvwprintw(game_win, room->foods[i].cord.y, room->foods[i].cord.x, "♣");
                wattroff(game_win, COLOR_PAIR(7));
                // wattroff(game_win, COLOR_PAIR(color_pair_num));
            }
            else{
                mvwprintw(game_win, room->foods[i].cord.y, room->foods[i].cord.x, "♣");
            }
            wattroff(game_win, COLOR_PAIR(7));
        }
    }
    for(int i = 0; i < room->shieldCount; i++){
        if(room->shields[i].isUsed == false){
            wattron(game_win, COLOR_PAIR(19));
            if((room->shields[i].cord.x - game->player->cord.x <= 1 && game->player->cord.x - room->shields[i].cord.x<= 1)
            && (room->shields[i].cord.y - game->player->cord.y <= 1 && game->player->cord.y - room->shields[i].cord.y <= 1)){
                // wattron(game_win, COLOR_PAIR(color_pair_num));
                wattron(game_win, COLOR_PAIR(19));
                mvwprintw(game_win, room->shields[i].cord.y, room->shields[i].cord.x, "🛡");
                wattroff(game_win, COLOR_PAIR(19));
                // wattroff(game_win, COLOR_PAIR(color_pair_num));
            }
            else{
                mvwprintw(game_win, room->shields[i].cord.y, room->shields[i].cord.x, "🛡");
            }
            wattroff(game_win, COLOR_PAIR(19));
        }
    }
    for(int i = 0; i < room->goldCount; i++){
        if(room->golds[i].isUsed == false){
            if((room->golds[i].cord.x - game->player->cord.x <= 1 && game->player->cord.x - room->golds[i].cord.x <= 1)
            && (room->golds[i].cord.y - game->player->cord.y <= 1 && game->player->cord.y - room->golds[i].cord.y <= 1)){
                if(room->golds[i].type == 'n'){
                    wattron(game_win, COLOR_PAIR(12));
                    // wattron(game_win, COLOR_PAIR(color_pair_num));
                    mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "¤");
                    // wattron(game_win, COLOR_PAIR(color_pair_num));
                    wattroff(game_win, COLOR_PAIR(12));
                }
                else if(room->golds[i].type == 'b'){
                    mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "🌑");
                }
            }
            else{
                if(room->golds[i].type == 'n'){
                    wattron(game_win, COLOR_PAIR(12));
                    mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "¤");
                    wattroff(game_win, COLOR_PAIR(12));
                }
                else if(room->golds[i].type == 'b'){
                    mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "🌑");
                }
            }
        }
    }
    for(int i = 0; i < room->spellCount; i++){
        if(room->spells[i].isUsed == false){
            if(room->spells[i].type == 's'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍾");
            }
            else if(room->spells[i].type == 'p'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍷");
            }
            else if(room->spells[i].type == 'h'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍸");
            }
        }
    }
    for(int i = 0; i < room->specialfoodCount; i++){
        if(room->specialfoods[i].isUsed == false){
            if(room->specialfoods[i].type == 'n' || room->specialfoods[i].type == 'r'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍓");
            }
            else if(room->specialfoods[i].type == 'p'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍘");
            }
            else if(room->specialfoods[i].type == 's'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍟");
            }
        }
    }
    for(int i = 0; i < room->weaponCount; i++){
        if(room->weapons[i].isUsed == false){
            if(room->weapons[i].type == 'm'){
                wattron(game_win, COLOR_PAIR(16));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U00002692');
                wattroff(game_win, COLOR_PAIR(16));
            }
            else if(room->weapons[i].type == 'd'){
                wattron(game_win, COLOR_PAIR(10));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U0001F5E1');
                wattroff(game_win, COLOR_PAIR(10));
            }
            else if(room->weapons[i].type == 'w'){
                wattron(game_win, COLOR_PAIR(17));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "⚚");
                wattroff(game_win, COLOR_PAIR(17));
                //  L'\U0001FA84'
            }
            else if(room->weapons[i].type == 'a'){
                wattron(game_win, COLOR_PAIR(18));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U000027B3');
                wattroff(game_win, COLOR_PAIR(18));
            }
            else if(room->weapons[i].type == 's'){
                wattron(game_win, COLOR_PAIR(8));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U00002694');
                wattroff(game_win, COLOR_PAIR(8));
            }
        }
    }
    for(int i = 0; i < room->trapCount; i++){
        if(player->currentWeapon.type == 's' && player->weapons[4].count >= 2 
        &&(room->traps[i].cord.x - game->player->cord.x <= 1 && game->player->cord.x - room->traps[i].cord.x <= 1)
        && (room->traps[i].cord.y - game->player->cord.y <= 1 && game->player->cord.y - room->traps[i].cord.y <= 1)){
            wattron(game_win, COLOR_PAIR(color_pair_num));
            mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, "^");
            wattroff(game_win, COLOR_PAIR(color_pair_num));
            room->traps[i].isVisible == true;
        }
        if(room->traps[i].isVisible == true){
            wattron(game_win, COLOR_PAIR(6));
            if((room->traps[i].cord.x - game->player->cord.x <= 1 && game->player->cord.x - room->traps[i].cord.x <= 1)
            && (room->traps[i].cord.y - game->player->cord.y <= 1 && game->player->cord.y - room->traps[i].cord.y <= 1)){
                mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, "^");
            }
            else{
                mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, "^");
            }
            wattroff(game_win, COLOR_PAIR(6));
        }
    }
    for(int i = 0; i < room->doorCount; i++){
        if(room->doors[i].type == 'n'){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "+");
        }
        else if(room->doors[i].type == 's' && room->doors[i].password == 1){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "‽");
        }
        else if(room->doors[i].type == 'l' || room->doors[i].type == 'h'){
            if(room->doors[i].is_open == false){
                wattron(game_win, COLOR_PAIR(6));
            }
            else{
                wattron(game_win, COLOR_PAIR(7));
            }
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "☯");
            if(room->doors[i].is_open == false){
                wattroff(game_win, COLOR_PAIR(6));
            }
            else{
                wattroff(game_win, COLOR_PAIR(7));
            }
            wattron(game_win, COLOR_PAIR(color_pair_num));
            mvwprintw(game_win, room->password_generator.y, room->password_generator.x, "&");
            wattroff(game_win, COLOR_PAIR(color_pair_num));
            if(room->doors[i].type == 'h'){
                wattron(game_win, COLOR_PAIR(color_pair_num));
                mvwprintw(game_win, room->password_generator2.y, room->password_generator2.x, "&");
                wattroff(game_win, COLOR_PAIR(color_pair_num));
            }
        }
    }
    if(room->window.x != 0 && room->window.x != 0){
        mvwprintw(game_win, room->window.y, room->window.x, "=");
    }
    for(int i = 0; i < room->enemyCount; i++){
        if(room->enemies[i].isVisible == true){
            wattron(game_win, COLOR_PAIR(14));
            mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x, "%c", room->enemies[i].type - 32);
            wattroff(game_win, COLOR_PAIR(14));
        }
    }
    if(room_number == 0 && game->currentLevel != 0){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, "<");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    else if(room->stairCount == 1){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, ">");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    else if(room->type == 'm' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(6));
    }
    if(room->keyCount == 1){
        wattron(game_win, COLOR_PAIR(12));
        mvwprintw(game_win, room->ancientkey.y, room->ancientkey.x, "Δ");
        wattroff(game_win, COLOR_PAIR(12));
    }
    wattroff(game_win, COLOR_PAIR(color_pair_num));
    color_pair_num = temp;
}
void draw_ranged_attacked_room(WINDOW *game_win, Room *room, int room_number, int dx, int dy) {
    // wattron(game_win, COLOR_PAIR(color_pair_num));
    if(is_last_room){
        draw_last_room(game_win, room, room_number);
        return;
    }
    int temp = color_pair_num;
    if(color_pair_num == 0){
        color_pair_num = 14;
    }
    if(room->type == 's' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(15));
    }
    else if(room->type == 'p' && room->isVisible == true){
        if(room_number == game->player->room_num){
            in_spell = 1;
        }
        wattron(game_win, COLOR_PAIR(17));
    }
    else if(room->type == 't' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(12));
    }
    if(room->type == 'm' && room->isVisible == true && all_print == 0){
        in_nightmare = 1;
        return;
    }
    if(room->type == 'm' && room->isVisible == true){
        wattron(game_win, COLOR_PAIR(6));
    }
    all_print = 1;
    in_nightmare = 0;
    room->lock_attemps = 0;
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            if(y == room->y && x == room->x){
                mvwprintw(game_win, y, x, "┌");
            }
            else if(y == room->y + room->height - 1 && x == room->x){
                mvwprintw(game_win, y, x, "└");
            }
            else if(y == room->y && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┐");
            }
            else if(y == room->y + room->height - 1 && x == room->x + room->width - 1){
                mvwprintw(game_win, y, x, "┘");
            }
            else if (y == room->y) {
                mvwprintw(game_win, y, x, "─");
            } else if (y == room->y + room->height - 1) {
                mvwprintw(game_win, y, x, "─");
            } else if (x == room->x || x == room->x + room->width - 1) {
                mvwprintw(game_win, y, x, "│");
            } else {
                if(room->type == 't' && room->isVisible == true){
                    wattron(game_win, COLOR_PAIR(12));
                }
                if(dx == 1){
                    if(x > game->player->cord.x && y == game->player->cord.y && x - game->player->cord.x <= game->player->currentWeapon.range){
                        wattron(game_win, COLOR_PAIR(color_pair_num));
                    }
                }
                else if(dx == -1 && y == game->player->cord.y && game->player->cord.x - x <= game->player->currentWeapon.range){
                    if(x < game->player->cord.x){
                        wattron(game_win, COLOR_PAIR(color_pair_num));
                    }
                }
                else if(dy == 1 && x == game->player->cord.x && y - game->player->cord.y <= game->player->currentWeapon.range){
                    if(y > game->player->cord.y){
                        wattron(game_win, COLOR_PAIR(color_pair_num));
                    }
                }
                else if(dy == -1 && x == game->player->cord.x && game->player->cord.y - y <= game->player->currentWeapon.range){
                    if(y < game->player->cord.y){
                        wattron(game_win, COLOR_PAIR(color_pair_num));
                    }
                }
                mvwprintw(game_win, y, x, ".");
                if(dx == 1 && y == game->player->cord.y){
                    if(x > game->player->cord.x && x - game->player->cord.x <= game->player->currentWeapon.range){
                        wattroff(game_win, COLOR_PAIR(color_pair_num));
                    }
                }
                else if(dx == -1 && y == game->player->cord.y){
                    if(x < game->player->cord.x && game->player->cord.x - x <= game->player->currentWeapon.range){
                        wattroff(game_win, COLOR_PAIR(color_pair_num));
                    }
                }
                else if(dy == 1 && x == game->player->cord.x){
                    if(y > game->player->cord.y && y - game->player->cord.y <= game->player->currentWeapon.range){
                        wattroff(game_win, COLOR_PAIR(color_pair_num));
                    }
                }
                else if(dy == -1 && x == game->player->cord.x && game->player->cord.y - y <= game->player->currentWeapon.range){
                    if(y < game->player->cord.y){
                        wattroff(game_win, COLOR_PAIR(color_pair_num));
                    }
                }
            }
        }
    }
    for(int i = 0; i < room->pillarCount; i++){
        wattron(game_win, COLOR_PAIR(16));
        mvwprintw(game_win, room->pillars[i].y, room->pillars[i].x, "▒");
        wattroff(game_win, COLOR_PAIR(16));
    }
    for(int i = 0; i < room->foodCount; i++){
        if(room->foods[i].isUsed == false){
            wattron(game_win, COLOR_PAIR(7));
            mvwprintw(game_win, room->foods[i].cord.y, room->foods[i].cord.x, "♣");
            wattroff(game_win, COLOR_PAIR(7));
        }
    }
    for(int i = 0; i < room->shieldCount; i++){
        if(room->shields[i].isUsed == false){
            wattron(game_win, COLOR_PAIR(19));
            mvwprintw(game_win, room->shields[i].cord.y, room->shields[i].cord.x, "🛡");
            wattroff(game_win, COLOR_PAIR(19));
        }
    }
    for(int i = 0; i < room->goldCount; i++){
        if(room->golds[i].isUsed == false){
            if(room->golds[i].type == 'n'){
                wattron(game_win, COLOR_PAIR(12));
                mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "¤");
                wattroff(game_win, COLOR_PAIR(12));
            }
            else if(room->golds[i].type == 'b'){
                mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "🌑");
            }
        }
    }
    for(int i = 0; i < room->spellCount; i++){
        if(room->spells[i].isUsed == false){
            if(room->spells[i].type == 's'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍾");
            }
            else if(room->spells[i].type == 'p'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍷");
            }
            else if(room->spells[i].type == 'h'){
                mvwprintw(game_win, room->spells[i].cord.y, room->spells[i].cord.x, "🍸");
            }
        }
    }
    for(int i = 0; i < room->specialfoodCount; i++){
        if(room->specialfoods[i].isUsed == false){
            if(room->specialfoods[i].type == 'n' || room->specialfoods[i].type == 'r'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍓");
            }
            else if(room->specialfoods[i].type == 'p'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍘");
            }
            else if(room->specialfoods[i].type == 's'){
                mvwprintw(game_win, room->specialfoods[i].cord.y, room->specialfoods[i].cord.x, "🍟");
            }
        }
    }
    for(int i = 0; i < room->weaponCount; i++){
        if(room->weapons[i].isUsed == false){
            if(room->weapons[i].type == 'm'){
                wattron(game_win, COLOR_PAIR(16));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U00002692');
                wattroff(game_win, COLOR_PAIR(16));
            }
            else if(room->weapons[i].type == 'd'){
                wattron(game_win, COLOR_PAIR(10));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U0001F5E1');
                wattroff(game_win, COLOR_PAIR(10));
            }
            else if(room->weapons[i].type == 'w'){
                wattron(game_win, COLOR_PAIR(17));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "⚚");
                wattroff(game_win, COLOR_PAIR(17));
                //  L'\U0001FA84'
            }
            else if(room->weapons[i].type == 'a'){
                wattron(game_win, COLOR_PAIR(18));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U000027B3');
                wattroff(game_win, COLOR_PAIR(18));
            }
            else if(room->weapons[i].type == 's'){
                wattron(game_win, COLOR_PAIR(8));
                mvwprintw(game_win, room->weapons[i].cord.y, room->weapons[i].cord.x, "%lc", L'\U00002694');
                wattroff(game_win, COLOR_PAIR(8));
            }
        }
    }
    for(int i = 0; i < room->trapCount; i++){
        if(room->traps[i].isVisible == true || t_check){
            wattron(game_win, COLOR_PAIR(6));
            mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, "^");
            wattroff(game_win, COLOR_PAIR(6));
        }
    }
    for(int i = 0; i < room->doorCount; i++){
        if(room->doors[i].type == 'n'){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "+");
        }
        else if(room->doors[i].type == 'l' || room->doors[i].type == 'h'){
            if(room->doors[i].is_open == false){
                wattron(game_win, COLOR_PAIR(6));
            }
            else{
                wattron(game_win, COLOR_PAIR(7));
            }
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "☯");
            if(room->doors[i].is_open == false){
                wattroff(game_win, COLOR_PAIR(6));
            }
            else{
                wattroff(game_win, COLOR_PAIR(7));
            }
            wattron(game_win, COLOR_PAIR(color_pair_num));
            mvwprintw(game_win, room->password_generator.y, room->password_generator.x, "&");
            wattroff(game_win, COLOR_PAIR(color_pair_num));
            if(room->doors[i].type == 'h'){
                wattron(game_win, COLOR_PAIR(color_pair_num));
                mvwprintw(game_win, room->password_generator2.y, room->password_generator2.x, "&");
                wattroff(game_win, COLOR_PAIR(color_pair_num));
            }
        }
        else if((room->doors[i].type == 's' && room->doors[i].password == 1) || t_check){
            mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "‽");
        }
    }
    for(int i = 0; i < room->enemyCount; i++){
        if(room->enemies[i].isVisible == true){
            wattron(game_win, COLOR_PAIR(14));
            mvwprintw(game_win, room->enemies[i].cord.y, room->enemies[i].cord.x, "%c", room->enemies[i].type - 32);
            wattroff(game_win, COLOR_PAIR(14));
        }
    }
    if(room->window.x != 0 && room->window.x != 0){
        mvwprintw(game_win, room->window.y, room->window.x, "=");
    }
    if(room_number == 0 && game->currentLevel != 0){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, "<");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    else if(room->stairCount == 1){
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, ">");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
    }
    if(room->type == 's' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(15));
    }
    else if(room->type == 'p' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(17));
    }
    else if(room->type == 'm' && room->isVisible == true){
        wattroff(game_win, COLOR_PAIR(6));
    }
    if(room->keyCount == 1){
    
        wattron(game_win, COLOR_PAIR(12));
        mvwprintw(game_win, room->ancientkey.y, room->ancientkey.x, "Δ");
        wattroff(game_win, COLOR_PAIR(12));
    }
    color_pair_num = temp;
    // wattroff(game_win, COLOR_PAIR(color_pair_num));
}
void clear_room(WINDOW *game_win, Room *room, int room_number) {
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            mvwprintw(game_win, y, x, " ");
        }
    }
}
void print_map(WINDOW *game_win){
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width - BORDER_PADDING;
    int end_y = map_height - BORDER_PADDING;
    srand(time(NULL));
    // keypad(game_win, TRUE);
    game->levels[game->currentLevel]->rooms[0]->isVisible = true;
    for(int i = start_y; i < map_height; i++){
        for(int j = start_x; j < map_width; j++){
            if(game->whole_map[game->currentLevel][i][j].isVisible == true){
                wattron(game_win, COLOR_PAIR(3));
                if(game->whole_map[game->currentLevel][i][j].item == '#'){
                    // mvwprintw(game_win, i, j, "%lc", game->whole_map[game->currentLevel][i][j].item);
                    mvwprintw(game_win, i, j, "▓");
                }
                wattroff(game_win, COLOR_PAIR(3));
            }
            else{
                mvwprintw(game_win, i, j, " ");
            }
        }
    }
    for(int i = 0; i < game->levels[game->currentLevel]->roomsCount; i++){
        if(game->levels[game->currentLevel]->rooms[i]->isVisible == true){
            draw_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
            if(game->levels[game->currentLevel]->rooms[i]->type == 'm'){
                draw_nightmare_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                in_nightmare = 0;
            }
        }
    }
    int k = 0;
    // for(int i = start_y; i < map_height; i++){
    //     for(int j = start_x; j < map_width; j++){
    //         mvwprintw(game_win, game->whole_map[k]->cord.y, game->whole_map[k]->cord.x, "%lc", game->whole_map[k]->item);
    //         k++;
    //     }
    // }
    // for(int i = start_y; i < map_height; i++){
    //     for(int j = start_x; j < map_width; j++){
    //         mvwprintw(game_win, i, j, "%lc", game->whole_map[game->currentLevel][i][j]);
    //     }
    // }
    if(!strcmp(game->player->color, " Yellow ")){
        color_pair_num = 1;
        wattron(game_win, COLOR_PAIR(1));
    }
    else if(!strcmp(game->player->color, " Red ")){
        color_pair_num = 6;
        wattron(game_win, COLOR_PAIR(6));
    }
    else if(!strcmp(game->player->color, " Blue ")){
        color_pair_num = 9;
        wattron(game_win, COLOR_PAIR(9));
    }
    else if(!strcmp(game->player->color, " Green ")){
        color_pair_num = 7;
        wattron(game_win, COLOR_PAIR(7));
    }
    mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "@");
    box(game_win, 0, 0);
    if(!strcmp(game->player->color, " Yellow ")){
        wattroff(game_win, COLOR_PAIR(1));
    }
    else if(!strcmp(game->player->color, " Red ")){
        wattroff(game_win, COLOR_PAIR(6));
    }
    else if(!strcmp(game->player->color, " Blue ")){
        wattroff(game_win, COLOR_PAIR(9));
    }
    else if(!strcmp(game->player->color, " Green ")){
        wattroff(game_win, COLOR_PAIR(7));
    }
    wattron(game_win, COLOR_PAIR(color_pair_num));
    if(color_pair_num == 0){
        wattron(game_win, COLOR_PAIR(14));
    }
    mvwprintw(game_win, map_height - 8, map_width - 22, "█████████████████");
    mvwprintw(game_win, map_height - 7, map_width - 22, "█████████████████");
    mvwprintw(game_win, map_height - 6, map_width - 22, "█████████████████");
    mvwprintw(game_win, map_height - 5, map_width - 22, "███  ████  ██████");
    mvwprintw(game_win, map_height - 4, map_width - 22, "█████    ████████");
    mvwprintw(game_win, map_height - 3, map_width - 22, "█████████████████");
    wattroff(game_win, COLOR_PAIR(color_pair_num));
    wattroff(game_win, COLOR_PAIR(14));
    wrefresh(game_win);
}
void print_all_map(WINDOW *game_win){
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING;
    srand(time(NULL));
    for(int i = start_y; i < map_height; i++){
        for(int j = start_x; j < map_width; j++){
            if(game->whole_map[game->currentLevel][i][j].isVisible == true){
                wattron(game_win, COLOR_PAIR(3));
            }
            if(game->whole_map[game->currentLevel][i][j].isVisible == false){
                wattron(game_win, COLOR_PAIR(10));
            }
            if(game->whole_map[game->currentLevel][i][j].item == '#'){
                // mvwprintw(game_win, i, j, "%lc", game->whole_map[game->currentLevel][i][j].item);
                mvwprintw(game_win, i, j, "▓");
            }
            wattroff(game_win, COLOR_PAIR(3));
            wattroff(game_win, COLOR_PAIR(10));
        }
    }
    for(int i = 0; i < game->levels[game->currentLevel]->roomsCount; i++){
        if(game_win, game->levels[game->currentLevel]->rooms[i]->isVisible == false){
            wattron(game_win, COLOR_PAIR(20));
        }
        all_print = 1;
        draw_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
        if(game_win, game->levels[game->currentLevel]->rooms[i]->isVisible == false){
            wattroff(game_win, COLOR_PAIR(20));
        }
    }
    if(!strcmp(game->player->color, " Yellow ")){
        color_pair_num = 1;
        wattron(game_win, COLOR_PAIR(1));
    }
    else if(!strcmp(game->player->color, " Red ")){
        color_pair_num = 6;
        wattron(game_win, COLOR_PAIR(6));
    }
    else if(!strcmp(game->player->color, " Blue ")){
        color_pair_num = 9;
        wattron(game_win, COLOR_PAIR(9));
    }
    else if(!strcmp(game->player->color, " Green ")){
        color_pair_num = 7;
        wattron(game_win, COLOR_PAIR(7));
    }
    mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "@");
    box(game_win, 0, 0);
    if(!strcmp(game->player->color, " Yellow ")){
        wattroff(game_win, COLOR_PAIR(1));
    }
    else if(!strcmp(game->player->color, " Red ")){
        wattroff(game_win, COLOR_PAIR(6));
    }
    else if(!strcmp(game->player->color, " Blue ")){
        wattroff(game_win, COLOR_PAIR(9));
    }
    else if(!strcmp(game->player->color, " Green ")){
        wattroff(game_win, COLOR_PAIR(7));
    }
    all_print = 0;
    wrefresh(game_win);
}
void continue_corridor(WINDOW *game_win, int y, int x, int a){
    Point next[4];
    next[0].x = x + 1;
    next[0].y = y;
    next[1].x = x;
    next[1].y = y + 1;
    next[2].x = x - 1;
    next[2].y = y;
    next[3].x = x;
    next[3].y = y - 1;
    if(a == 5){
        in_nightmare = 0;
        return;
    }
    else{
        for(int i = 0; i < 4; i++){
            if(game->whole_map[game->currentLevel][next[i].y][next[i].x].item == '#' 
            || game->whole_map[game->currentLevel][next[i].y][next[i].x].item == '+'){
                if(game->whole_map[game->currentLevel][next[i].y][next[i].x].item == '#'){
                    wattron(game_win, COLOR_PAIR(3));
                }
                // mvwprintw(game_win, next[i].y, next[i].x, "%lc", game->whole_map[game->currentLevel][next[i].y][next[i].x].item);
                if(game->whole_map[game->currentLevel][next[i].y][next[i].x].item == '#'){
                    mvwprintw(game_win, next[i].y, next[i].x, "▓");
                }
                else{
                    mvwprintw(game_win, next[i].y, next[i].x, "%lc", game->whole_map[game->currentLevel][next[i].y][next[i].x].item);
                }
                game->whole_map[game->currentLevel][next[i].y][next[i].x].isVisible = true;
                if(game->whole_map[game->currentLevel][next[i].y][next[i].x].item == '#'){
                    wattroff(game_win, COLOR_PAIR(3));
                }
                continue_corridor(game_win, next[i].y, next[i].x, a + 1);
            }
        }
    }
}
void printf_diff_from_map(WINDOW *game_win){
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '?'){
        diff_from_map = '?';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "‽");
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
        diff_from_map = '#';
        wattron(game_win, COLOR_PAIR(3));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "▓");
        wattroff(game_win, COLOR_PAIR(3));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '%'){
        diff_from_map = '%';
        wattron(game_win, COLOR_PAIR(7));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "♣");
        wattroff(game_win, COLOR_PAIR(7));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '>'){
        diff_from_map = '>';
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, ">");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '<'){
        diff_from_map = '<';
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "<");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'A'){
        diff_from_map = 'A';
        wattron(game_win, COLOR_PAIR(12));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "Δ");
        wattroff(game_win, COLOR_PAIR(12));
        return;
    }
    if(is_enemy(game->player->cord.y, game->player->cord.x)){
        diff_from_map = 'A';
        wattron(game_win, COLOR_PAIR(14));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%c", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
        wattroff(game_win, COLOR_PAIR(14));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '$'){
        diff_from_map = '$';
        wattron(game_win, COLOR_PAIR(12));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "¤");
        wattroff(game_win, COLOR_PAIR(12));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '^'){
        diff_from_map = '^';
        wattron(game_win, COLOR_PAIR(6));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "^");
        wattroff(game_win, COLOR_PAIR(6));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'L'){
        diff_from_map = 'L';
        wattron(game_win, COLOR_PAIR(lock_color));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "☯");
        wattroff(game_win, COLOR_PAIR(lock_color));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '&'){
        diff_from_map = '&';
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "&");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '*' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '*'){
        diff_from_map = '*';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🌑");
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '*' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '*'){
        diff_from_map = '*';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🌑");
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'E' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == 'E'){
        diff_from_map = 'E';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🍾");
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'E' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == 'E'){
        diff_from_map = 'E';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🍾");
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'P' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == 'P'){
        diff_from_map = 'P';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🍷");
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'P' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == 'P'){
        diff_from_map = 'P';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🍷");
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'H' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == 'H'){
        diff_from_map = 'H';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🍸");
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'H' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == 'H'){
        diff_from_map = 'H';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🍸");
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '6' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '6'){
        diff_from_map = '6';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🍓");
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '6' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '6'){
        diff_from_map = '6';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🍓");
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '9' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '9'){
        diff_from_map = '9';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🍓");
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '9' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '9'){
        diff_from_map = '9';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🍓");
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '7' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '7'){
        diff_from_map = '7';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🍘");
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '7' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '7'){
        diff_from_map = '7';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🍘");
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '8' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '8'){
        diff_from_map = '8';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🍟");
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '8' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '8'){
        diff_from_map = '9';
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🍟");
        return;
    }
    // else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '1' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '1'){
    //     diff_from_map = '1';
    //     wattron(game_win, COLOR_PAIR(16));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, "⚒");
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 2, ".");
    //     wattroff(game_win, COLOR_PAIR(16));
    //     return;
    // }
    // else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '1' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '1'){
    //     diff_from_map = '1';
    //     wattron(game_win, COLOR_PAIR(16));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "⚒");
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x, ".");
    //     wattroff(game_win, COLOR_PAIR(16));
    //     return;
    // }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '1'){
        diff_from_map = '1';
        wattron(game_win, COLOR_PAIR(16));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "⚒");
        wattroff(game_win, COLOR_PAIR(16));
        return;
    }
    // else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '2' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '2'){
    //     diff_from_map = '2';
    //     wattron(game_win, COLOR_PAIR(10));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, "%lc", L'\U0001F5E1');
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 2, ".");
    //     wattroff(game_win, COLOR_PAIR(10));
    //     return;
    // }
    // else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '2' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '2'){
    //     diff_from_map = '2';
    //     wattron(game_win, COLOR_PAIR(10));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "%lc", L'\U0001F5E1');
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x, ".");
    //     wattroff(game_win, COLOR_PAIR(10));
    //     return;
    // }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '2'){
        diff_from_map = '2';
        wattron(game_win, COLOR_PAIR(10));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", L'\U0001F5E1');
        wattroff(game_win, COLOR_PAIR(10));
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '3' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '3'){
        diff_from_map = '3';
        wattron(game_win, COLOR_PAIR(17));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, "⚚");
        wattroff(game_win, COLOR_PAIR(17));
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '3' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '3'){
        diff_from_map = '3';
        wattron(game_win, COLOR_PAIR(17));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "⚚");
        wattroff(game_win, COLOR_PAIR(17));
        return;
    }
    // else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '4' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '4'){
    //     diff_from_map = '4';
    //     wattron(game_win, COLOR_PAIR(18));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, "%lc", L'\U000027B3');
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 2, ".");
    //     wattroff(game_win, COLOR_PAIR(18));
    //     return;
    // }
    // else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '4' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '4'){
    //     diff_from_map = '4';
    //     wattron(game_win, COLOR_PAIR(18));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "%lc", L'\U000027B3');
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x, ".");
    //     wattroff(game_win, COLOR_PAIR(18));
    //     return;
    // }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '4'){
        diff_from_map = '4';
        wattron(game_win, COLOR_PAIR(18));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", L'\U000027B3');
        wattroff(game_win, COLOR_PAIR(18));
        return;
    }
    // else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '5' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '5'){
    //     diff_from_map = '5';
    //     wattron(game_win, COLOR_PAIR(8));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, "%lc", L'\U00002694');
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 2, ".");
    //     wattroff(game_win, COLOR_PAIR(8));
    //     return;
    // }
    // else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '5' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '5'){
    //     diff_from_map = '5';
    //     wattron(game_win, COLOR_PAIR(8));
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "%lc", L'\U00002694');
    //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x, ".");
    //     wattroff(game_win, COLOR_PAIR(8));
    //     return;
    // }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '5'){
        diff_from_map = '5';
        wattron(game_win, COLOR_PAIR(8));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", L'\U00002694');
        wattroff(game_win, COLOR_PAIR(8));
        return;
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '!' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '!'){
        diff_from_map = '!';
        wattron(game_win, COLOR_PAIR(19));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "🛡");
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, " ");
        wattroff(game_win, COLOR_PAIR(19));
        return;
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '!' && game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '!'){
        diff_from_map = '!';
        wattron(game_win, COLOR_PAIR(19));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, "🛡");
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, " ");
        wattroff(game_win, COLOR_PAIR(19));
        return;
    }
}
void lost_game_screen(time_t start_time) {
    Mix_HaltMusic();
    game->player->can_be_saved = 0;
    game->player->games_count++;
    int win_height = LINES, win_width = COLS;
    int startx = (COLS - win_width) / 2;
    int starty = (LINES - win_height) / 2;
    WINDOW *lost_win = newwin(win_height, win_width, starty, startx);

    int y_kaka = 16;
    wattron(lost_win, COLOR_PAIR(color_pair_num));
    box(lost_win, 0, 0);
    // for(int i = 1; i < LINES; i++){
    //     for(int j = 0; j < 5; j++){
    //         mvwprintw(lost_win, i, j, "__");
    //     }
    // }
    // for(int i = 1; i < LINES; i++){
    //     for(int j = COLS - 6; j < COLS - 2; j++){
    //         mvwprintw(lost_win, i, j, "__");
    //     }
    // }
    mvwprintw(lost_win, LINES / 2 - 15, (COLS - 45) / 2 - 4, "                    ███████████████                              ");
    mvwprintw(lost_win, LINES / 2 - 14, (COLS - 45) / 2 - 4, "                █████████ ████ ████                                       ");
    mvwprintw(lost_win, LINES / 2 - 13, (COLS - 45) / 2 - 4, "                    ████  ████  ███                                    ");
    mvwprintw(lost_win, LINES / 2 - 12, (COLS - 45) / 2 - 4, "                ███████████ ███████                                   ");
    mvwprintw(lost_win, LINES / 2 - 11, (COLS - 45) / 2 - 4, "                    ███████████████                                    ");
    mvwprintw(lost_win, LINES / 2 - 10, (COLS - 45) / 2 - 4, "                ██  ████      █████                                          ");
    mvwprintw(lost_win, LINES / 2 - 9, (COLS - 45) / 2 - 4, "              █████████ ██████ ███                                        ");
    mvwprintw(lost_win, LINES / 2 - 8, (COLS - 45) / 2 - 4, "              ██████                                                           ");
    mvwprintw(lost_win, LINES / 2 - 7, (COLS - 45) / 2 - 4, "              ████  █████████████                                  ");
    mvwprintw(lost_win, LINES / 2 - 6, (COLS - 45) / 2 - 4, "                ██  ██████  █████                                      ");
    mvwprintw(lost_win,  LINES / 2 - 5, (COLS - 45) / 2 - 4, "                ██  ██████  █████                                            ");
    wattroff(lost_win, COLOR_PAIR(color_pair_num));
    wattron(lost_win, COLOR_PAIR(6));
    mvwprintw(lost_win, LINES / 2 - 24 + y_kaka, (COLS - 45) / 2 - 12, "          __     ");
    mvwprintw(lost_win, LINES / 2 - 23 + y_kaka, (COLS - 45) / 2 - 12, "      |__|__     ");
    mvwprintw(lost_win, LINES / 2 - 22 + y_kaka, (COLS - 45) / 2 - 12, "       __|  |     ");
    wattroff(lost_win, COLOR_PAIR(6));
    wattron(lost_win, COLOR_PAIR(5));
    mvwprintw(lost_win, LINES / 2 - 21 + y_kaka, (COLS - 45) / 2 - 12, "      ___|__     ");
    mvwprintw(lost_win, LINES / 2 - 20 + y_kaka, (COLS - 45) / 2 - 12, "     /      \\   ");
    mvwprintw(lost_win, LINES / 2 - 19 + y_kaka, (COLS - 45) / 2 - 12, "    /  _  _  \\ ");
    mvwprintw(lost_win, LINES / 2 - 18 + y_kaka, (COLS - 45) / 2 - 12, "   |          |   ");
    mvwprintw(lost_win, LINES / 2 - 17 + y_kaka, (COLS - 45) / 2 - 12, "   |     |    |   ");
    mvwprintw(lost_win, LINES / 2 - 16 + y_kaka, (COLS - 45) / 2 - 12, "   |  ______  |   ");
    mvwprintw(lost_win, LINES / 2 - 15 + y_kaka, (COLS - 45) / 2 - 12, "  |   |____|  |  ");
    mvwprintw(lost_win, LINES / 2 - 14 + y_kaka, (COLS - 45) / 2 - 12, " |            | ");
    mvwprintw(lost_win, LINES / 2 - 13 + y_kaka, (COLS - 45) / 2 - 12, " \\           /  ");
    mvwprintw(lost_win, LINES / 2 - 12 + y_kaka, (COLS - 45) / 2 - 12, "  \\ ______ /  ");
    wattroff(lost_win, COLOR_PAIR(5));
    wattron(lost_win, COLOR_PAIR(6));
    mvwprintw(lost_win, LINES / 2 - 22 + y_kaka, (COLS - 45) / 2 + 39, "       ___      ");
    mvwprintw(lost_win, LINES / 2 - 21 + y_kaka, (COLS - 45) / 2 + 39, "      _|*|__     ");
    wattroff(lost_win, COLOR_PAIR(6));
    wattron(lost_win, COLOR_PAIR(5));
    mvwprintw(lost_win, LINES / 2 - 20 + y_kaka, (COLS - 45) / 2 + 39, "     /      \\   ");
    mvwprintw(lost_win, LINES / 2 - 19 + y_kaka, (COLS - 45) / 2 + 39, "    /  _  _  \\ ");
    mvwprintw(lost_win, LINES / 2 - 18 + y_kaka, (COLS - 45) / 2 + 39, "   |  |_||_|  |   ");
    mvwprintw(lost_win, LINES / 2 - 17 + y_kaka, (COLS - 45) / 2 + 39, "   |    _|    |   ");
    mvwprintw(lost_win, LINES / 2 - 16 + y_kaka, (COLS - 45) / 2 + 39, "   |  ______  |   ");
    mvwprintw(lost_win, LINES / 2 - 15 + y_kaka, (COLS - 45) / 2 + 39, "   |    |+|    |  ");
    mvwprintw(lost_win, LINES / 2 - 14 + y_kaka, (COLS - 45) / 2 + 39, "   |            | ");
    mvwprintw(lost_win, LINES / 2 - 13 + y_kaka, (COLS - 45) / 2 + 39, "    \\           /  ");
    mvwprintw(lost_win, LINES / 2 - 12 + y_kaka, (COLS - 45) / 2 + 39, "      \\ ______ /  ");
    wattroff(lost_win, COLOR_PAIR(5));
    wattron(lost_win, COLOR_PAIR(3));\
    int y_kaka1 = y_kaka - 1;
    mvwprintw(lost_win, LINES / 2 - 19 + y_kaka1, (COLS - 45) / 2 + 8, "You're out of hp and died,");
    time_t end_time;
    time(&end_time);
    mvwprintw(lost_win, LINES / 2 - 18 + y_kaka1, (COLS - 45) / 2 + 9, " Better luck next time!");
    mvwprintw(lost_win, LINES / 2 - 17 + y_kaka1, (COLS - 45) / 2 + 10, "   Your point: %.0lf", (game->player->play_time + difftime(end_time, start_time)) * 5 + game->player->gold * 10);
    mvwprintw(lost_win, LINES / 2 - 16 + y_kaka1, (COLS - 45) / 2 + 9, "  Press Enter to exit.");
    wattroff(lost_win, COLOR_PAIR(3));
    wattron(lost_win, COLOR_PAIR(10));
    mvwprintw(lost_win, LINES / 2 - 15 + y_kaka1, (COLS - 45) / 2 + 6, "  ,-=-.       ______     _");
    mvwprintw(lost_win, LINES / 2 - 14 + y_kaka1, (COLS - 45) / 2 + 5, "  /  +  \\     />----->  _|1|_");
    mvwprintw(lost_win, LINES / 2 - 13 + y_kaka1, (COLS - 45) / 2 + 5, "  | ~~~ |    // -/- /  |_ H _|");
    mvwprintw(lost_win, LINES / 2 - 12 + y_kaka1, (COLS - 45) / 2 + 5, "  |R.I.P|   //  /  /     |S|");
    mvwprintw(lost_win, LINES / 2 - 11 + y_kaka1, (COLS - 45) / 2 + 2, "\vV,,|_____|V,//_____/VvV,v,|_|/,,vhjwv/,");
    wattroff(lost_win, COLOR_PAIR(10));
    wattron(lost_win, COLOR_PAIR(7));
    mvwprintw(lost_win, LINES / 2 - 11 + y_kaka1, (COLS - 45) / 2 + 2, "\vV,,");
    mvwprintw(lost_win, LINES / 2 - 11 + y_kaka1, (COLS - 45) / 2 + 14, "V,");
    mvwprintw(lost_win, LINES / 2 - 11 + y_kaka1, (COLS - 45) / 2 + 24, "VvV,v,");
    mvwprintw(lost_win, LINES / 2 - 11 + y_kaka1, (COLS - 45) / 2 + 33, "/,,vhjwv/,");
    wattroff(lost_win, COLOR_PAIR(7));
    wattron(lost_win, COLOR_PAIR(color_pair_num));
    int a = 30;
    if(color_pair_num == 0){
            wattron(lost_win, COLOR_PAIR(14));
    }
    mvwprintw(lost_win, LINES - 8 - a, COLS - 21, "█████████████████");
    mvwprintw(lost_win, LINES - 7 - a, COLS - 21, "█████████████████");
    mvwprintw(lost_win, LINES - 6 - a, COLS - 21, "█████████████████");
    mvwprintw(lost_win, LINES - 5 - a, COLS - 21, "███  ████  ██████");
    mvwprintw(lost_win, LINES - 4 - a, COLS - 21, "█████    ████████");
    mvwprintw(lost_win, LINES - 3 - a, COLS - 21, "█████████████████");
    wattroff(lost_win, COLOR_PAIR(color_pair_num));
    wattroff(lost_win, COLOR_PAIR(14));
    wattron(lost_win, COLOR_PAIR(16));
    mvwprintw(lost_win, LINES - 2 - a, COLS - 24, "╰────────────────────╯");
    mvwprintw(lost_win, LINES - 1 - a, COLS - 24, "⧹⧹;|_|;|_|;|;|_|;|_⧸⧸");
    mvwprintw(lost_win, LINES - 0 - a, COLS - 23, "\\\\.    .   ,   .  /");
    mvwprintw(lost_win, LINES + 1 - a, COLS - 23, " \\\\:  .   ,  . , /");
    mvwprintw(lost_win, LINES + 2 - a, COLS - 23, "  \\\\:  .    . , /");
    mvwprintw(lost_win, LINES + 3 - a, COLS - 23, "   \\\\:  .  . , /");
    mvwprintw(lost_win, LINES + 4 - a, COLS - 23, "    ||:  ,    |");
    mvwprintw(lost_win, LINES + 5 - a, COLS - 23, "    ||:    .  |");
    mvwprintw(lost_win, LINES + 6 - a, COLS - 23, "    ||:    ,  |");
    mvwprintw(lost_win, LINES + 7 - a, COLS - 23, "    ||: .    ,|");
    mvwprintw(lost_win, LINES + 8 - a, COLS - 23, "    ||:   *   |");
    mvwprintw(lost_win, LINES + 9 - a, COLS - 23, "    ||:  `    |");
    mvwprintw(lost_win, LINES + 10 - a, COLS - 23, "    ||:   -   |");
    mvwprintw(lost_win, LINES + 11 - a, COLS - 23, "    ||: ,  '  |");
    mvwprintw(lost_win, LINES + 12 - a, COLS - 23, ".o_//       ,   \\_o.");
    wattroff(lost_win, COLOR_PAIR(16));
    wattron(lost_win, COLOR_PAIR(3));
    mvwprintw(lost_win, LINES - 11 - a, COLS - 76, "╭───────────────────────────────────────────────────╮");
    mvwprintw(lost_win, LINES - 10 - a, COLS - 76, "│ Hey my friend, I guess you had bad luck!          │");
    mvwprintw(lost_win, LINES - 9 - a, COLS - 76, "│ No problem! You can surely make it next time.🔥   │");
    mvwprintw(lost_win, LINES - 8 - a, COLS - 76, "│ After all those quotes I hope you enjoyed the game│");
    mvwprintw(lost_win, LINES - 7 - a, COLS - 76, "│ And most important thing is the friendship that   │");
    mvwprintw(lost_win, LINES - 6 - a, COLS - 76, "│ I made with you along this journey %s", game->player->name);
    mvwprintw(lost_win, LINES - 6 - a, COLS - 24, "│");
    mvwprintw(lost_win, LINES - 5 - a, COLS - 76, "│ See you soon. 😉                                 _⧹");
    mvwprintw(lost_win, LINES - 4 - a, COLS - 76, "╰─────────────────────────────────────────────────┘  ¯");
    wattroff(lost_win, COLOR_PAIR(3));
    wrefresh(lost_win);
    sleep(3);
    while(1){
        char c = wgetch(lost_win);
        if(c == 10){
            break;
        }
    }
    wclear(lost_win);
    wrefresh(lost_win);
    delwin(lost_win);
    endwin();
}
void win_game_screen(time_t start_time) {
    Mix_HaltMusic();
    game->player->can_be_saved = 0;
    game->player->games_count;
    int win_height = LINES, win_width = COLS;
    int startx = (COLS - win_width) / 2;
    int starty = (LINES - win_height) / 2;
    WINDOW *win_win = newwin(win_height, win_width, starty, startx);
    int y_kaka = 18;
    wattron(win_win, COLOR_PAIR(color_pair_num));
    box(win_win, 0, 0);
    mvwprintw(win_win, LINES / 2 - 14, (COLS - 45) / 2 - 4, "                    ███████████████                              ");
    mvwprintw(win_win, LINES / 2 - 13, (COLS - 45) / 2 - 4, "                █████████ █████ ███                                       ");
    mvwprintw(win_win, LINES / 2 - 12, (COLS - 45) / 2 - 4, "                    ████  ████  ███                                    ");
    mvwprintw(win_win, LINES / 2 - 11, (COLS - 45) / 2 - 4, "                ███████████ ███████                                   ");
    mvwprintw(win_win, LINES / 2 - 10, (COLS - 45) / 2 - 4, "                    ███ ██████ ████                                    ");
    mvwprintw(win_win, LINES / 2 - 9, (COLS - 45) / 2 - 4, "                ██  ████       ████                                          ");
    mvwprintw(win_win, LINES / 2 - 8, (COLS - 45) / 2 - 4, "              ████████████████████                                        ");
    mvwprintw(win_win, LINES / 2 - 7, (COLS - 45) / 2 - 4, "              ██████                                                           ");
    mvwprintw(win_win, LINES / 2 - 6, (COLS - 45) / 2 - 4, "              ████  █████████████                                  ");
    mvwprintw(win_win, LINES / 2 - 5, (COLS - 45) / 2 - 4, "                ██  ██████  █████                                      ");
    mvwprintw(win_win,  LINES / 2 - 4, (COLS - 45) / 2 - 4, "                ██  ██████  █████                                            ");
    wattroff(win_win, COLOR_PAIR(color_pair_num));
    wattron(win_win, COLOR_PAIR(6));
    mvwprintw(win_win, LINES / 2 - 24 + y_kaka, (COLS - 45) / 2 - 12, "          __     ");
    mvwprintw(win_win, LINES / 2 - 23 + y_kaka, (COLS - 45) / 2 - 12, "      |__|__     ");
    mvwprintw(win_win, LINES / 2 - 22 + y_kaka, (COLS - 45) / 2 - 12, "       __|  |     ");
    wattroff(win_win, COLOR_PAIR(6));
    wattron(win_win, COLOR_PAIR(5));
    mvwprintw(win_win, LINES / 2 - 21 + y_kaka, (COLS - 45) / 2 - 12, "      ___|__     ");
    mvwprintw(win_win, LINES / 2 - 20 + y_kaka, (COLS - 45) / 2 - 12, "     /      \\   ");
    mvwprintw(win_win, LINES / 2 - 19 + y_kaka, (COLS - 45) / 2 - 12, "    /  _  _  \\ ");
    mvwprintw(win_win, LINES / 2 - 18 + y_kaka, (COLS - 45) / 2 - 12, "   |          |   ");
    mvwprintw(win_win, LINES / 2 - 17 + y_kaka, (COLS - 45) / 2 - 12, "   |     |    |   ");
    mvwprintw(win_win, LINES / 2 - 16 + y_kaka, (COLS - 45) / 2 - 12, "   |  ______  |   ");
    mvwprintw(win_win, LINES / 2 - 15 + y_kaka, (COLS - 45) / 2 - 12, "  |   |____|  |  ");
    mvwprintw(win_win, LINES / 2 - 14 + y_kaka, (COLS - 45) / 2 - 12, " |            | ");
    mvwprintw(win_win, LINES / 2 - 13 + y_kaka, (COLS - 45) / 2 - 12, " \\           /  ");
    mvwprintw(win_win, LINES / 2 - 12 + y_kaka, (COLS - 45) / 2 - 12, "  \\ ______ /  ");
    wattroff(win_win, COLOR_PAIR(5));
    wattron(win_win, COLOR_PAIR(6));
    mvwprintw(win_win, LINES / 2 - 22 + y_kaka, (COLS - 45) / 2 + 39, "       ___      ");
    mvwprintw(win_win, LINES / 2 - 21 + y_kaka, (COLS - 45) / 2 + 39, "      _|*|__     ");
    wattroff(win_win, COLOR_PAIR(6));
    wattron(win_win, COLOR_PAIR(5));
    mvwprintw(win_win, LINES / 2 - 20 + y_kaka, (COLS - 45) / 2 + 39, "     /      \\   ");
    mvwprintw(win_win, LINES / 2 - 19 + y_kaka, (COLS - 45) / 2 + 39, "    /  _  _  \\ ");
    mvwprintw(win_win, LINES / 2 - 18 + y_kaka, (COLS - 45) / 2 + 39, "   |  |_||_|  |   ");
    mvwprintw(win_win, LINES / 2 - 17 + y_kaka, (COLS - 45) / 2 + 39, "   |    _|    |   ");
    mvwprintw(win_win, LINES / 2 - 16 + y_kaka, (COLS - 45) / 2 + 39, "   |  ______  |   ");
    mvwprintw(win_win, LINES / 2 - 15 + y_kaka, (COLS - 45) / 2 + 39, "   |    |+|    |  ");
    mvwprintw(win_win, LINES / 2 - 14 + y_kaka, (COLS - 45) / 2 + 39, "   |            | ");
    mvwprintw(win_win, LINES / 2 - 13 + y_kaka, (COLS - 45) / 2 + 39, "    \\           /  ");
    mvwprintw(win_win, LINES / 2 - 12 + y_kaka, (COLS - 45) / 2 + 39, "      \\ ______ /  ");
    wattroff(win_win, COLOR_PAIR(5));
    wattron(win_win, COLOR_PAIR(3));\
    int y_kaka1 = y_kaka - 3;
    mvwprintw(win_win, LINES / 2 - 18 + y_kaka1, (COLS - 45) / 2 + 8, "       You made it!");
    time_t end_time;
    time(&end_time);
    mvwprintw(win_win, LINES / 2 - 17 + y_kaka1, (COLS - 45) / 2 + 10, "    Your point: %.0lf", (game->player->play_time + difftime(end_time, start_time)) * 5 + game->player->gold * 10);
    mvwprintw(win_win, LINES / 2 - 16 + y_kaka1, (COLS - 45) / 2 + 11, " Press enter to exit.");
    wattroff(win_win, COLOR_PAIR(3));
    wattron(win_win, COLOR_PAIR(14));
    mvwprintw(win_win, LINES / 2 - 15 + y_kaka1, (COLS - 45) / 2 + 15, "   _,     ,_");
    mvwprintw(win_win, LINES / 2 - 14 + y_kaka1, (COLS - 45) / 2 + 15, " .'/  ,_   \\'.");
    mvwprintw(win_win, LINES / 2 - 13 + y_kaka1, (COLS - 45) / 2 + 15, "|  \\__('>__/  |");
    mvwprintw(win_win, LINES / 2 - 12 + y_kaka1, (COLS - 45) / 2 + 15, "\\   - ,  -    /");
    mvwprintw(win_win, LINES / 2 - 11 + y_kaka1, (COLS - 45) / 2 + 15, " '-..__ __..-'");
    mvwprintw(win_win, LINES / 2 - 10 + y_kaka1, (COLS - 45) / 2 + 15, "      /_\\");
    mvwprintw(win_win, LINES / 2 - 9 + y_kaka1, (COLS - 45) / 2 + 15, "      m m");
    wattroff(win_win, COLOR_PAIR(14));
    wattron(win_win, COLOR_PAIR(color_pair_num));
    if(color_pair_num == 0){
        wattron(win_win, COLOR_PAIR(14));
    }
    else{
        wattron(win_win, COLOR_PAIR(color_pair_num));
    }
    int a = 30;
    mvwprintw(win_win, LINES - 8 - a, COLS - 21, "█████████████████");
    mvwprintw(win_win, LINES - 7 - a, COLS - 21, "█████████████████");
    mvwprintw(win_win, LINES - 6 - a, COLS - 21, "█████████████████");
    mvwprintw(win_win, LINES - 5 - a, COLS - 21, "███  ████  ██████");
    mvwprintw(win_win, LINES - 4 - a, COLS - 21, "█████    ████████");
    mvwprintw(win_win, LINES - 3 - a, COLS - 21, "█████████████████");
    wattroff(win_win, COLOR_PAIR(color_pair_num));
    wattroff(win_win, COLOR_PAIR(14));
    wattron(win_win, COLOR_PAIR(16));
    mvwprintw(win_win, LINES - 2 - a, COLS - 24, "╰────────────────────╯");
    mvwprintw(win_win, LINES - 1 - a, COLS - 24, "⧹⧹;|_|;|_|;|;|_|;|_⧸⧸");
    mvwprintw(win_win, LINES - 0 - a, COLS - 23, "\\\\.    .   ,   .  /");
    mvwprintw(win_win, LINES + 1 - a, COLS - 23, " \\\\:  .   ,  . , /");
    mvwprintw(win_win, LINES + 2 - a, COLS - 23, "  \\\\:  .    . , /");
    mvwprintw(win_win, LINES + 3 - a, COLS - 23, "   \\\\:  .  . , /");
    mvwprintw(win_win, LINES + 4 - a, COLS - 23, "    ||:  ,    |");
    mvwprintw(win_win, LINES + 5 - a, COLS - 23, "    ||:    .  |");
    mvwprintw(win_win, LINES + 6 - a, COLS - 23, "    ||:    ,  |");
    mvwprintw(win_win, LINES + 7 - a, COLS - 23, "    ||: .    ,|");
    mvwprintw(win_win, LINES + 8 - a, COLS - 23, "    ||:   *   |");
    mvwprintw(win_win, LINES + 9 - a, COLS - 23, "    ||:  `    |");
    mvwprintw(win_win, LINES + 10 - a, COLS - 23, "    ||:   -   |");
    mvwprintw(win_win, LINES + 11 - a, COLS - 23, "    ||: ,  '  |");
    mvwprintw(win_win, LINES + 12 - a, COLS - 23, ".o_//       ,   \\_o.");
    wattroff(win_win, COLOR_PAIR(16));
    wattron(win_win, COLOR_PAIR(3));
    mvwprintw(win_win, LINES - 11 - a, COLS - 76, "╭───────────────────────────────────────────────────╮");
    mvwprintw(win_win, LINES - 10 - a, COLS - 76, "│ Hey my friend, I knew you could make it through!  │");
    mvwprintw(win_win, LINES - 9 - a, COLS - 76, "│ Well done! You are that one worthy hero.🔥        │");
    mvwprintw(win_win, LINES - 8 - a, COLS - 76, "│ After all those quotes I hope you enjoyed the game│");
    mvwprintw(win_win, LINES - 7 - a, COLS - 76, "│ And most important thing is the friendship that   │");
    mvwprintw(win_win, LINES - 6 - a, COLS - 76, "│ I made with you along this journey %s =)", game->player->name);
    mvwprintw(win_win, LINES - 6 - a, COLS - 24, "│");
    mvwprintw(win_win, LINES - 5 - a, COLS - 76, "│ See you soon. 😉                                 _⧹");
    mvwprintw(win_win, LINES - 4 - a, COLS - 76, "╰─────────────────────────────────────────────────┘  ¯");
    wattroff(win_win, COLOR_PAIR(3));
    wrefresh(win_win);
    sleep(5);
    while(1){
        char c = wgetch(win_win);
        if(c == 10){
            break;
        }
    }
    wclear(win_win);
    wrefresh(win_win);
    delwin(win_win);
    endwin();
}
int valid_move_check(int y, int x){
    if(game->whole_map[game->currentLevel][y][x].item != '|' 
    && game->whole_map[game->currentLevel][y][x].item != '-' 
    && game->whole_map[game->currentLevel][y][x].item != '_' 
    && game->whole_map[game->currentLevel][y][x].item != 'O' 
    && game->whole_map[game->currentLevel][y][x].item != '=' 
    && game->whole_map[game->currentLevel][y][x].item != 0
    && game->whole_map[game->currentLevel][y][x].item != ' '){
        if(game->whole_map[game->currentLevel][y][x].item == 'L'){
            for(int i = 0; i < game->levels[game->currentLevel]->rooms[game->player->room_num]->doorCount; i++){
                if(game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[i].is_open == false 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[i].cord.x == x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[i].cord.y == y){
                    return 0;
                }
            }
        }
        return 1;
    }
    else{
        return 0;
    }
}
void enemy_move_can_pass_throw_corridors(Enemy *enemy, WINDOW *game_win){
    if(enemy->canMove == false || enemy->isAlive == false){
        return;
    }
    int dx = 0, dy = 0;
    game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = last_item;
    if(last_item == '#'){
        wattron(game_win, COLOR_PAIR(3));
        mvwprintw(game_win, enemy->cord.y, enemy->cord.x, "#");
        wattroff(game_win, COLOR_PAIR(3));
    }
    if(enemy->cord.x > game->player->cord.x){
        dx = -1;
    }
    if(enemy->cord.y > game->player->cord.y){
        dy = -1;
    }
    if(enemy->cord.x < game->player->cord.x){
        dx = 1;
    }
    if(enemy->cord.y < game->player->cord.y){
        dy = 1;
    }
    if(rand() % 2 && dx != 0){
        if(valid_move_check(enemy->cord.y, enemy->cord.x + dx)){
            enemy->cord.x += dx;
            last_item = game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item;
            game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = enemy->type - 32;
            return;
        }
    }
    else{
        if(valid_move_check(enemy->cord.y + dy, enemy->cord.x)){
            if(dy == 0){
                if(valid_move_check(enemy->cord.y + dy, enemy->cord.x + dx)){
                    enemy->cord.x += dx;
                }
            }
            enemy->cord.y += dy;
            last_item = game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item;
            game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = enemy->type - 32;
            return;
        }
    }
    if(rand() % 2 && dx != 0){
        if(valid_move_check(enemy->cord.y, enemy->cord.x - dx)){
            enemy->cord.x -= dx;
            last_item = game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item;
            game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = enemy->type - 32;
            return;
        }
    }
    else{
        if(valid_move_check(enemy->cord.y - dy, enemy->cord.x)){
            if(dy == 0){
                if(valid_move_check(enemy->cord.y - dy, enemy->cord.x - dx)){
                    enemy->cord.x -= dx;
                }
            }
            enemy->cord.y -= dy;
            last_item = game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item;
            game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = enemy->type - 32;
            return;
        }
    }
}
// Change Name above and below to change the moving way.
void enemy_move(Enemy *enemy, WINDOW *game_win){
    if(enemy->canMove == false || enemy->isAlive == false){
        return;
    }
    int dx = 0, dy = 0;
    game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = last_item;
    if(last_item == '#'){
        wattron(game_win, COLOR_PAIR(3));
        mvwprintw(game_win, enemy->cord.y, enemy->cord.x, "#");
        wattroff(game_win, COLOR_PAIR(3));
    }
    if(enemy->cord.x > game->player->cord.x){
        dx = -1;
    }
    if(enemy->cord.y > game->player->cord.y){
        dy = -1;
    }
    if(enemy->cord.x < game->player->cord.x){
        dx = 1;
    }
    if(enemy->cord.y < game->player->cord.y){
        dy = 1;
    }
    if(rand() % 2 && dx != 0){
        if(valid_move_check(enemy->cord.y, enemy->cord.x + dx)
        && game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x + dx].item != '+'
        && game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x + dx].item != 'L'
        && game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x + dx].item != '?'){
            enemy->cord.x += dx;
            last_item = game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item;
            game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = enemy->type - 32;
            return;
        }
    }
    else{
        if(valid_move_check(enemy->cord.y + dy, enemy->cord.x)
        && game->whole_map[game->currentLevel][enemy->cord.y + dy][enemy->cord.x].item != '+'
        && game->whole_map[game->currentLevel][enemy->cord.y + dy][enemy->cord.x].item != 'L'
        && game->whole_map[game->currentLevel][enemy->cord.y + dy][enemy->cord.x].item != '?'){
            if(dy == 0){
                if(valid_move_check(enemy->cord.y + dy, enemy->cord.x + dx)
                && game->whole_map[game->currentLevel][enemy->cord.y + dy][enemy->cord.x + dx].item != '+'
                && game->whole_map[game->currentLevel][enemy->cord.y + dy][enemy->cord.x + dx].item != 'L'
                && game->whole_map[game->currentLevel][enemy->cord.y + dy][enemy->cord.x + dx].item != '?'){
                    enemy->cord.x += dx;
                }
            }
            enemy->cord.y += dy;
            last_item = game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item;
            game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = enemy->type - 32;
            return;
        }
    }
    if(rand() % 2 && dx != 0){
        if(valid_move_check(enemy->cord.y, enemy->cord.x - dx)
        && game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x - dx].item != '+'
        && game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x - dx].item != 'L'
        && game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x - dx].item != '?'){
            enemy->cord.x -= dx;
            last_item = game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item;
            game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = enemy->type - 32;
            return;
        }
    }
    else{
        if(valid_move_check(enemy->cord.y - dy, enemy->cord.x)
        && game->whole_map[game->currentLevel][enemy->cord.y - dy][enemy->cord.x].item != '+'
        && game->whole_map[game->currentLevel][enemy->cord.y - dy][enemy->cord.x].item != 'L'
        && game->whole_map[game->currentLevel][enemy->cord.y - dy][enemy->cord.x].item != '?'){
            if(dy == 0){
                if(valid_move_check(enemy->cord.y - dy, enemy->cord.x - dx)
                && game->whole_map[game->currentLevel][enemy->cord.y - dy][enemy->cord.x - dx].item != '+'
                && game->whole_map[game->currentLevel][enemy->cord.y - dy][enemy->cord.x - dx].item != 'L'
                && game->whole_map[game->currentLevel][enemy->cord.y - dy][enemy->cord.x - dx].item != '?'){
                    enemy->cord.x -= dx;
                }
            }
            enemy->cord.y -= dy;
            last_item = game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item;
            game->whole_map[game->currentLevel][enemy->cord.y][enemy->cord.x].item = enemy->type - 32;
            return;
        }
    }
}
void entering_password() {
    int correct_password = game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].password;
    int correct_password2;
    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].type == 'h'){
        correct_password2 = game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].password2;
    }
    int input;
    int input2;
    int attempts = last_attemps;
    int choice;
    int highlight = 0;
    int win_height = 10, win_width = 42;
    int startx = (COLS - win_width) / 2;
    int starty = (LINES - win_height) / 2;
    WINDOW *password_win = newwin(win_height, win_width, starty, startx);
    wattron(password_win, COLOR_PAIR(color_pair_num));
    box(password_win, 0, 0);
    wattroff(password_win, COLOR_PAIR(color_pair_num));
    char *options[] = {"Use Ancient Key", "Enter Password", "Exit"};
    keypad(password_win, TRUE);
    int color = color_pair_num;
    if(attempts == 1){
        color = 8;
    }
    else if(attempts == 2){
        color = 14;
    }
    else if(attempts == 3){
        color = 6;
    }
    while (1) {
        werase(password_win);
        wattron(password_win, COLOR_PAIR(color));
        box(password_win, 0, 0);
        wattroff(password_win, COLOR_PAIR(color));
        
        for (int i = 0; i < 3; i++) {
            if (i == highlight)
                wattron(password_win, A_REVERSE);
            if(i == 0){
                wattron(password_win, COLOR_PAIR(12));
            }
            else if(i == 1){
                wattron(password_win, COLOR_PAIR(color));
            }
            mvwprintw(password_win, i + 1, 1, "● %s", options[i]);
            wattroff(password_win, A_REVERSE);
            wattroff(password_win, COLOR_PAIR(12));
            wattroff(password_win, COLOR_PAIR(color));
        }
        wrefresh(password_win);

        choice = wgetch(password_win);

        switch (choice) {
            case KEY_UP:
                highlight = (highlight - 1 + 3) % 3;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 3;
                break;
            case 10:
                if (highlight == 0) {
                    if(game->player->acientKey <= 0){
                        mvwprintw(password_win, 5, 1, "You don't have any Ancient Keys!");
                    }
                    else{
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].is_open = true;
                        game->player->acientKey--;
                        if(rand() % 10 == 0){
                            mvwprintw(password_win, 5, 1, "Aww, bad luck, you broke your key :(");
                            game->player->brokenAcientKey++;
                        }
                        else{
                            wattron(password_win, COLOR_PAIR(12));
                            mvwprintw(password_win, 5, 1, "You used the ancient key!");
                            wattroff(password_win, COLOR_PAIR(12));
                            wrefresh(password_win);
                            wgetch(password_win);
                            wclear(password_win);
                            wrefresh(password_win);
                            delwin(password_win);
                            endwin();
                            last_attemps = 0;
                            return;
                        }
                    }
                    wrefresh(password_win);
                    wgetch(password_win);
                    break;
                } else if (highlight == 1) {
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].type == 'l'){
                        while (attempts < 3) {
                            werase(password_win);
                            // wattron(password_win, COLOR_PAIR(color_pair_num));
                            wattron(password_win, COLOR_PAIR(color));
                            box(password_win, 0, 0);
                            // wattroff(password_win, COLOR_PAIR(color_pair_num));
                            mvwprintw(password_win, 1, 1, "Enter Password: ");
                            wrefresh(password_win);
                            echo();
                            char input_str[20];
                            mvwgetnstr(password_win, 2, 1, input_str, 20);
                            noecho();
                            input = atoi(input_str);
                            // mvwgetnstr(password_win, 2, 1, (char*) &input, 20);

                            if (input == correct_password || input == 1111) {
                                wattron(password_win, COLOR_PAIR(7));
                                mvwprintw(password_win, 3, 1, "Access Granted!");
                                wattroff(password_win, COLOR_PAIR(7));
                                game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].is_open = true;
                                wrefresh(password_win);
                                wgetch(password_win);
                                wclear(password_win);
                                wrefresh(password_win);
                                delwin(password_win);
                                endwin();
                                // game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = 0;
                                last_attemps = 0;
                                return;
                            } else {
                                attempts++;
                                if(attempts == 1){
                                    color = 8;
                                }
                                else if(attempts == 2){
                                    color = 14;
                                }
                                else if(attempts == 3){
                                    color = 6;
                                    break;
                                }
                                last_attemps = attempts;
                                wattron(password_win, COLOR_PAIR(color));
                                mvwprintw(password_win, 3, 1, "Incorrect Password. Try again.");
                                mvwprintw(password_win, 4, 1, "It might be reversed!");
                                wattroff(password_win, COLOR_PAIR(color));
                                // wgetch(password_win);
                                break;
                            }
                        }

                        if (attempts == 3) {
                            last_attemps = 3;
                            wattron(password_win, COLOR_PAIR(6));
                            mvwprintw(password_win, 3, 1, "Security Mode Activated. Access Denied!");
                            wattroff(password_win, COLOR_PAIR(6));
                        }
                        
                        wrefresh(password_win);
                        wgetch(password_win);
                        break;
                    }
                    else{
                        while (attempts < 3) {
                            werase(password_win);
                            // wattron(password_win, COLOR_PAIR(color_pair_num));
                            wattron(password_win, COLOR_PAIR(color));
                            box(password_win, 0, 0);
                            // wattroff(password_win, COLOR_PAIR(color_pair_num));
                            mvwprintw(password_win, 1, 1, "Enter Password: ");
                            mvwprintw(password_win, 2, 1, "Enter Password: ");
                            wrefresh(password_win);
                            echo();
                            char input_str[20];
                            mvwgetnstr(password_win, 1, 17, input_str, 20);
                            char input2_str[20];
                            mvwgetnstr(password_win, 2, 17, input2_str, 20);
                            noecho();
                            input = atoi(input_str);
                            input2 = atoi(input2_str);
                            // mvwgetnstr(password_win, 2, 1, (char*) &input, 20);

                            if ((input == correct_password && input2 == correct_password2) || input == 1111) {
                                wattron(password_win, COLOR_PAIR(7));
                                mvwprintw(password_win, 3, 1, "Access Granted!");
                                wattroff(password_win, COLOR_PAIR(7));
                                game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].is_open = true;
                                wrefresh(password_win);
                                wgetch(password_win);
                                wclear(password_win);
                                wrefresh(password_win);
                                delwin(password_win);
                                endwin();
                                last_attemps = 0;
                                return;
                            } else {
                                attempts++;
                                if(attempts == 1){
                                    color = 8;
                                }
                                else if(attempts == 2){
                                    color = 14;
                                }
                                else if(attempts == 3){
                                    color = 6;
                                    break;
                                }
                                last_attemps = attempts;
                                wattron(password_win, COLOR_PAIR(color));
                                mvwprintw(password_win, 3, 1, "Incorrect Password. Try again.");
                                wattroff(password_win, COLOR_PAIR(color));
                                // wgetch(password_win);
                                break;
                            }
                        }
                        if (attempts == 3) {
                            last_attemps = 3;
                            wattron(password_win, COLOR_PAIR(6));
                            mvwprintw(password_win, 3, 1, "Security Mode Activated. Access Denied!");
                            wattroff(password_win, COLOR_PAIR(6));
                        }
                        wrefresh(password_win);
                        wgetch(password_win);
                        break;
                    }
                }
                else if (highlight == 2) {
                    wclear(password_win);
                    wrefresh(password_win);
                    delwin(password_win);
                    endwin();
                    // game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = 1;
                    last_attemps = attempts;
                    return;
                }
                break;
        }
        wattroff(password_win, COLOR_PAIR(color));
    }
    // game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = 1;
    last_attemps = attempts;
    wclear(password_win);
    wrefresh(password_win);
    delwin(password_win);
    endwin();
}
void enemy_find(int y, int x, WINDOW *game_win){
    if(is_enemy(y, x)){
        for(int i = 0; i < game->levels[game->currentLevel]->rooms[game->player->room_num]->enemyCount; i++){
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == x 
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == y
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isAlive == true){
                is_enemy_or_not = game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type;
                // if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isVisible == false){
                    draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isVisible = true;
                // }
                if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 's'){
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves != 1){
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves = 0;
                    }
                    // enemy_move(&game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i]);
                    // draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], i);
                }
                else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'g'){
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves < 5
                    && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves != 1){
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves++;
                        // enemy_move(&game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i]);
                        // draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], i);
                    }
                    else{
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves = 0;
                    }
                }
                else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'u'){
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves < 5 
                    && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves != 1){
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves = 1;
                        // enemy_move(&game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i]);
                        // draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], i);
                    }
                    else{
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves = 0;
                    }
                }
            }
        }
    }
}
void after_find_enemy(WINDOW *game_win){
    for(int i = 0; i < game->levels[game->currentLevel]->rooms[game->player->room_num]->enemyCount; i++){
        if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 's'
        && (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves == 0
        || game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves == 1)
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isAlive == true){
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves == 1){
                enemy_move(&game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i], game_win);
                draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], i);
            }
            else{
                game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves = 1;
            }
        }
        else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'i'){
            enemy_move(&game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i], game_win);
            draw_last_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], i);
        }
        else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'g'){
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves <= 5 
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves > 0){
                game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves++;
                enemy_move(&game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i], game_win);
                draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], i);
            }
            // else{
            //     game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves = 0;
            // }
        }
        else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'u'){
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves <= 5 
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves > 0){
                game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves++;
                enemy_move(&game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i], game_win);
                draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], i);
            }
            // else{
            //     game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].moves = 0;
            // }
        }
        if(((game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x + 1
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x - 1
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y - 1)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y + 1))
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'i'){
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].health > 50){
                int message = rand() % 4;
                int damage = 10;
                mvwprintw(game_win, 2, 82, "                                                                                                  ");
                is_enemy_or_not = 0;
                wattron(game_win, COLOR_PAIR(17));
                if(message == 0){
                    mvwprintw(game_win, 2, 82, "  Hey, you're near the Immortal and damaged by %d :<", damage);
                }
                else if(message == 1){
                    mvwprintw(game_win, 2, 82, "  Keep your distance! You've been damaged for %d🎆", damage);
                }
                else if(message == 2){
                    mvwprintw(game_win, 2, 82, "  Oh crap, Immortal Monster damaged you by it zone for %d D=", damage);
                }
                else if(message == 3){
                    mvwprintw(game_win, 2, 82, "  Watch it! Final Monster hit %d damage to you with it's shield.", damage);
                }
                if(game->player->shield > 0){
                    if(game->player->shield - damage <= 0){
                        game->player->shield = 0;
                        mvwprintw(game_win, 3, 82, "                                                                                                ");
                        mvwprintw(game_win, 3, 82, "Your shield broke! From now on, you will get health damage.");
                    }
                    else{
                        game->player->shield -= damage;
                    }
                }
                else{
                    game->player->health -= damage;
                }
                wattroff(game_win, COLOR_PAIR(17));
            }
        }
        else if((game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x + 1
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x - 1
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y - 1)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y + 1))
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isAlive == true){
            // if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isVisible == false){
                draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
                game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isVisible = true;
            // }
            int message = rand() % 4;
            int damage = game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].damage * ((difficulty_efficiency + 2) / 3);
            mvwprintw(game_win, 2, 82, "                                                                                                  ");
            is_enemy_or_not = 0;
            wattron(game_win, COLOR_PAIR(14));
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'i'){
                damage = 25;
            }
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].health <= 50 
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'i'){
                damage = 30;
            }
            if(message == 0){
                mvwprintw(game_win, 2, 82, "  Ouch, you've been hit by %s and recieved %d damage!", 
                game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].name, damage);
            }
            else if(message == 1){
                mvwprintw(game_win, 2, 82, "  Ah shit, you've been hit by %s and recieved %d damage🗿", 
                game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].name, damage);
            }
            else if(message == 2){
                mvwprintw(game_win, 2, 82, "  Damn it, you've been hit by %s and recieved %d damage D=", 
                game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].name, damage);
            }
            else if(message == 3){
                mvwprintw(game_win, 2, 82, "  Watch it! you've been hit by %s and recieved %d damage :<", 
                game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].name, damage);
            }
            if(game->player->shield > 0){
                if(game->player->shield - damage <= 0){
                    game->player->shield = 0;
                    mvwprintw(game_win, 3, 82, "                                                                                                ");
                    mvwprintw(game_win, 3, 82, "Your shield broke! From now on, you will get health damage.");
                }
                else{
                    game->player->shield -= damage;
                }
            }
            else{
                game->player->health -= damage;
            }
            wattroff(game_win, COLOR_PAIR(14));
        }
        else if(((game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x + 1
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y + 1)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x - 1
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y - 1)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x + 1
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y - 1)
        || (game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == game->player->cord.x + 1
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == game->player->cord.y + 1))
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'i'
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].health > 50){
            int message = rand() % 4;
            int damage = 5;
            mvwprintw(game_win, 2, 82, "                                                                                                  ");
            is_enemy_or_not = 0;
            wattron(game_win, COLOR_PAIR(17));
            if(message == 0){
                mvwprintw(game_win, 2, 82, "  Hey, you're near the Immortal and damaged by %d :<", damage);
            }
            else if(message == 1){
                mvwprintw(game_win, 2, 82, "  Keep your distance! You've been damaged for %d🎆", damage);
            }
            else if(message == 2){
                mvwprintw(game_win, 2, 82, "  Oh crap, Immortal Monster damaged you by it zone for %d D=", damage);
            }
            else if(message == 3){
                mvwprintw(game_win, 2, 82, "  Watch it! Final Monster hit %d damage to you with it's shield.", damage);
            }
            if(game->player->shield > 0){
                if(game->player->shield - damage <= 0){
                    game->player->shield = 0;
                    mvwprintw(game_win, 3, 82, "                                                                                                ");
                    mvwprintw(game_win, 3, 82, "Your shield broke! From now on, you will get health damage.");
                }
                else{
                    game->player->shield -= damage;
                }
            }
            else{
                game->player->health -= damage;
            }
            wattroff(game_win, COLOR_PAIR(17));
        }
    }
}
void after_move(WINDOW *game_win){
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING + 5;
    wattron(game_win, COLOR_PAIR(color_pair_num));
    if(g_check == 0){
        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '%' && game->player->foodCount + 15 <= 150){
            for(int j = 0; j < game->levels[game->currentLevel]->rooms[game->player->room_num]->foodCount; j++){
                if(game->levels[game->currentLevel]->rooms[game->player->room_num]->foods[j].cord.x == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->foods[j].cord.y == game->player->cord.y
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->foods[j].isUsed != true){
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->foods[j].isUsed = true;
                    if(difficulty_efficiency >= 2 && rand() % 6 == 5){
                        poisonous_food_check = 1;
                        // for(int j = 0; j < game->player->foodCount; j++){
                        //     mvwprintw(game_win, map_height - 4 + j / 10, 5 + 2*j - 2*(j/10)*10, "🍖");
                        // }
                        mvwprintw(game_win, 3, 5, "                                                                                    ");
                        mvwprintw(game_win, 3, 5, "Ops! Poisonous food O_o");
                        game->player->health -= 5;
                    }
                    else{
                        mvwprintw(game_win, 3, 5, "                                                                                ");
                        mvwprintw(game_win, 3, 5, "Yummy, you've collected %d amount of food :3", game->levels[game->currentLevel]->rooms[game->player->room_num]->foods[j].health);
                        if(!strcmp(selected_music, "./Base_Sounds/silent.wav") && game->currentLevel != game->levelCount - 1){
                            Mix_PlayMusic(effects, 1);
                        }
                        game->player->foodCount += game->levels[game->currentLevel]->rooms[game->player->room_num]->foods[j].health;
                    }
                    // count_move = 1;
                    game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item = '.';
                }
            }
        }
        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '!' && game->player->shield < 100){
            for(int j = 0; j < game->levels[game->currentLevel]->rooms[game->player->room_num]->shieldCount; j++){
                if((game->levels[game->currentLevel]->rooms[game->player->room_num]->shields[j].cord.x == game->player->cord.x
                || game->levels[game->currentLevel]->rooms[game->player->room_num]->shields[j].cord.x + 1 == game->player->cord.x)
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->shields[j].cord.y == game->player->cord.y
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->shields[j].isUsed != true){
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->shields[j].isUsed = true;
                    mvwprintw(game_win, 3, 5, "                                                                                ");
                    mvwprintw(game_win, 3, 5, "Great, you've collected %d amount of shield !^-^!", game->levels[game->currentLevel]->rooms[game->player->room_num]->shields[j].amount);
                    game->player->shield += game->levels[game->currentLevel]->rooms[game->player->room_num]->shields[j].amount;
                    game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item = '.';
                    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '!'){
                        game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item = '.';
                        mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, ".");
                    }
                    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '!'){
                        game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item = '.';
                        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, ".");
                    }
                }
            }
        }
        else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'A'){
            game->levels[game->currentLevel]->rooms[game->player->room_num]->keyCount = 0;
            game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item = '.';
            game->player->acientKey++;
            mvwprintw(game_win, 3, 5, "                                                                             ");
            mvwprintw(game_win, 3, 5, "Nice! You've collected an Ancient Key Δ");
        }
        else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '$' 
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '*'){
            for(int j = 0; j < game->levels[game->currentLevel]->rooms[game->player->room_num]->goldCount; j++){
                if((game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].cord.x == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].cord.y == game->player->cord.y)
                || (game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].cord.x + 1 == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].cord.y == game->player->cord.y)
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].isUsed != true){
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].isUsed = true;
                    game->player->gold += game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].count * difficulty_efficiency;
                    game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item = '.';
                    mvwprintw(game_win, 3, 5, "                                                                               ");
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].type == 'b'){
                        // if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '*'){
                        //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, ".");
                        // }
                        // else{
                        //     mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, ".");
                        // }
                        mvwprintw(game_win, 3, 5, "Wow! You've collected a black gold with amount of %d 💰", 
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].count * difficulty_efficiency);
                        wattroff(game_win, COLOR_PAIR(color_pair_num));
                        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '*'){
                            game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item = '.';
                            mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, ".");
                        }
                        else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '*'){
                            game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item = '.';
                            mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, ".");
                        }
                        wattron(game_win, COLOR_PAIR(color_pair_num));
                    }
                    else{
                        mvwprintw(game_win, 3, 5, "You've collected a coin with amount of %d gold!", 
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->golds[j].count * difficulty_efficiency);
                    }
                }
            }
        }
        else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'E' 
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'P'
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'H'){
            for(int j = 0; j < game->levels[game->currentLevel]->rooms[game->player->room_num]->spellCount; j++){
                if((game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].cord.x == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].cord.y == game->player->cord.y)
                || (game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].cord.x + 1 == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].cord.y == game->player->cord.y)
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].isUsed != true){
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].isUsed = true;
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].type == 's'){
                        game->player->spells[0].count++;
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].type == 'p'){
                        game->player->spells[1].count++;
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].type == 'h'){
                        game->player->spells[2].count++;
                    }
                    // strcpy(game->player->spells[game->player->spellCount].name, game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].name);
                    game->player->spellCount++;
                    game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item = '.';
                    mvwprintw(game_win, 3, 5, "                                                                                  ");
                    mvwprintw(game_win, 3, 5, "You've collected a %s potion!", game->levels[game->currentLevel]->rooms[game->player->room_num]->spells[j].name);
                    wattroff(game_win, COLOR_PAIR(color_pair_num));
                    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == 'E'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == 'P'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == 'H'){
                        game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item = '.';
                        mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, ".");
                    }
                    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == 'E'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == 'P'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == 'H'){
                        game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item = '.';
                        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, ".");
                    }
                    wattron(game_win, COLOR_PAIR(color_pair_num));
                }
            }
        }
        else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '6' 
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '7'
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '8'
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '9'){
            for(int j = 0; j < game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoodCount; j++){
                if((game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].cord.x == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].cord.y == game->player->cord.y)
                || (game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].cord.x + 1 == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].cord.y == game->player->cord.y)
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].isUsed != true){
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].isUsed = true;
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].type == 'n'){
                        game->player->specialfoods[0].count++;
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].type == 'p'){
                        game->player->specialfoods[1].count++;
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].type == 's'){
                        game->player->specialfoods[2].count++;
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].type == 'r'){
                        game->player->specialfoods[3].count++;
                    }
                    game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item = '.';
                    if(!strcmp(selected_music, "./Base_Sounds/silent.wav") && game->currentLevel != game->levelCount - 1){
                        Mix_PlayMusic(effects, 1);
                    }
                    mvwprintw(game_win, 3, 5, "                                                                                  ");
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].type == 'n'){
                        mvwprintw(game_win, 3, 5, "You've collected a Special food!");
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].type == 'p'){
                        mvwprintw(game_win, 3, 5, "You've collected a Power food!");
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].type == 's'){
                        mvwprintw(game_win, 3, 5, "You've collected a Speed food!");
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->specialfoods[j].type == 'r'){
                        mvwprintw(game_win, 3, 5, "You've collected a Rotten food :(");
                        game->player->health -= 5;
                    }
                    wattroff(game_win, COLOR_PAIR(color_pair_num));
                    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '6'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '7'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '8'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '9'){
                        game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item = '.';
                        mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, ".");
                    }
                    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '6'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '7'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '8'
                    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '9'){
                        game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item = '.';
                        mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, ".");
                    }
                    wattron(game_win, COLOR_PAIR(color_pair_num));
                }
            }
        }
        else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '1' 
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '2'
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '3'
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '4'
        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '5'){
            for(int j = 0; j < game->levels[game->currentLevel]->rooms[game->player->room_num]->weaponCount; j++){
                if((game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].cord.x == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].cord.y == game->player->cord.y)
                || (game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].cord.x + 1 == game->player->cord.x 
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].cord.y == game->player->cord.y)
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].isUsed != true){
                    // game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].isUsed = true;
                    // if(game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].type == 'm'){
                    //     game->player->weapons[0].count++;
                    // }
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].type == 'd'){
                        int num = 25 - game->player->weapons[1].count;
                        if(game->player->weapons[1].count >= 25){
                            mvwprintw(game_win, 3, 5, "                                                                               ");
                            mvwprintw(game_win, 3, 5, "You can't have more than 25 daggers!");
                        }
                        else if(num < 10){
                            mvwprintw(game_win, 3, 5, "                                                   ");
                            mvwprintw(game_win, 3, 5, "You've collected %d Daggers!", num);
                            game->player->weapons[1].count += num;
                            game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count -= num;
                            game->player->weaponCount += num;
                        }
                        else if(num >= 10){
                            mvwprintw(game_win, 3, 5, "                                                   ");
                            mvwprintw(game_win, 3, 5, "You've collected %d Daggers!", game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count);
                            game->player->weapons[1].count += game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count;
                            game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count = 0;
                            game->player->weaponCount += game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count;
                        }
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].type == 'w'){
                        int num = 20 - game->player->weapons[2].count;
                        if(game->player->weapons[2].count >= 20){
                            mvwprintw(game_win, 3, 5, "                                                                               ");
                            mvwprintw(game_win, 3, 5, "You can't have more than 20 Magic Wands!");
                            draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
                        }
                        else if(num < 8){
                            mvwprintw(game_win, 3, 5, "                                                   ");
                            mvwprintw(game_win, 3, 5, "You've collected %d Magic Wands!", num);
                            game->player->weapons[2].count += num;
                            game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count -= num;
                            game->player->weaponCount += num;
                        }
                        else if(num >= 8){
                            mvwprintw(game_win, 3, 5, "                                                   ");
                            mvwprintw(game_win, 3, 5, "You've collected %d Magic Wands!", game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count);
                            game->player->weapons[2].count += game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count;
                            game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count = 0;
                            game->player->weaponCount += game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count;
                        }
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].type == 'a'){
                        int num = 50 - game->player->weapons[3].count;
                        if(game->player->weapons[3].count >= 50){
                            mvwprintw(game_win, 3, 5, "                                                                               ");
                            mvwprintw(game_win, 3, 5, "You can't have more than 50 Arrows!");
                        }
                        else if(num < 20){
                            mvwprintw(game_win, 3, 5, "                                                   ");
                            mvwprintw(game_win, 3, 5, "You've collected %d Arrows!", num);
                            game->player->weapons[3].count += num;
                            game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count -= num;
                            game->player->weaponCount += num;
                        }
                        else if(num >= 20){
                            mvwprintw(game_win, 3, 5, "                                                   ");
                            mvwprintw(game_win, 3, 5, "You've collected %d Arrows!", game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count);
                            game->player->weapons[3].count += game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count;
                            game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count = 0;
                            game->player->weaponCount += game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count;
                        }
                    }
                    else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].type == 's'){
                        if(game->player->weapons[4].count >= 5){
                            mvwprintw(game_win, 3, 5, "                                                                               ");
                            mvwprintw(game_win, 3, 5, "You can't have more than 5 sharpness on your Sword!");
                        }
                        else{
                            mvwprintw(game_win, 3, 5, "                                                   ");
                            game->player->weapons[4].count++;
                            mvwprintw(game_win, 3, 5, "You've upgraded your Sword to sharpness %d!", game->player->weapons[4].count);
                            game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count = 0;
                            game->player->weaponCount += 1;
                        }
                    }
                    // game->player->weaponCount++;
                    // mvwprintw(game_win, 3, 5, "                                                   ");
                    // mvwprintw(game_win, 3, 5, "You've collected a %s!", game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].name);
                    wattroff(game_win, COLOR_PAIR(color_pair_num));
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].count <= 0){
                        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '1' 
                        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '2'
                        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '3'
                        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '4'
                        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '5'){
                            game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item = '.';
                            mvwprintw(game_win, game->player->cord.y, game->player->cord.x + 1, ".");
                        }
                        else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '1' 
                        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '2'
                        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '3'
                        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '4'
                        || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '5'){
                            game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item = '.';
                            mvwprintw(game_win, game->player->cord.y, game->player->cord.x - 1, ".");
                        }
                        game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item = '.';
                        game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[j].isUsed = true;
                    }
                    wattron(game_win, COLOR_PAIR(color_pair_num));
                }
            }
        }
    }
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '^'){
        for(int j = 0; j < game->levels[game->currentLevel]->rooms[game->player->room_num]->trapCount; j++){
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->traps[j].cord.x == game->player->cord.x 
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->traps[j].cord.y == game->player->cord.y
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->traps[j].isVisible != true){
                game->levels[game->currentLevel]->rooms[game->player->room_num]->traps[j].isVisible = true;
                int damage = 5 * (rand() % (difficulty_efficiency + 1) + 1);
                if(rand() % 4 == 0 && game->currentLevel != game->levelCount - 1){
                    if(game->currentLevel != 6){
                        x_save = game->player->cord.x;
                        y_save = game->player->cord.y;
                        level_save = game->currentLevel;
                        game->currentLevel = 6;
                        is_battle = 1;
                        back_front = 1;
                    }
                    return;
                }
                else{
                    game->player->health -= damage;
                }
                mvwprintw(game_win, 3, 5, "                                                ");
                mvwprintw(game_win, 3, 5, "Ah, you've been hit by a trap and recieved %d damage :_(", damage);
            }
        }
    }
    else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '&'){
        int password;
        int password_copy;
        while(1){
            password = rand() % 10000;
            password_copy = password;
            if(rand() % 3 && game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].type != 'h'){
                int a1 = password % 10;
                password /= 10;
                int a2 = password % 10;
                password /= 10;
                int a3 = password % 10;
                password /= 10;
                int a4 = password % 10;
                password /= 10;
                password = a1 * 1000 + a2 * 100 + a3 * 10 + a4 * 1;
                // password = 1313;
            }
            if(password >= 1000 && password_copy >= 1000){
                break;
            }      
        }
        if(game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].type == 'h'){
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->password_generator.x == game->player->cord.x
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->password_generator.y == game->player->cord.y){
                game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].password = password;
                mvwprintw(game_win, 2, 5, "                                                                                      ");
                mvwprintw(game_win, 2, 5, "Lock password 1 is %d 🗝", password_copy);
            }
            else{
                game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].password2 = password;
                mvwprintw(game_win, 2, 5, "                                                                                      ");
                mvwprintw(game_win, 2, 5, "Lock password 2 is %d 🗝", password_copy);
            }
        }
        else{
            game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_door].password = password;
            mvwprintw(game_win, 2, 5, "                                                                                      ");
            mvwprintw(game_win, 2, 5, "Lock password is %d 🗝", password_copy);
        }
        is_pass_lock = 1;
        password_disapear = 0;
        time(&move_time_lock);
    }
    if(move_or_not){
        if(game->player->Health_effect > 0){
            game->player->Health_effect--;
        }
        if(game->player->Speed_effect > 0){
            game->player->Speed_effect--;
        }
        if(game->player->Power_effect > 0){
            game->player->Power_effect--;
        }
        if(game->player->Speed_effect % 2 == 0){
            // enemy_find(game->player->cord.y, game->player->cord.x, game_win);
            enemy_find(game->player->cord.y, game->player->cord.x + 1, game_win);
            enemy_find(game->player->cord.y, game->player->cord.x - 1, game_win);
            enemy_find(game->player->cord.y + 1, game->player->cord.x, game_win);
            enemy_find(game->player->cord.y - 1, game->player->cord.x, game_win);
            enemy_find(game->player->cord.y + 1, game->player->cord.x + 1, game_win);
            enemy_find(game->player->cord.y + 1, game->player->cord.x - 1, game_win);
            enemy_find(game->player->cord.y - 1, game->player->cord.x + 1, game_win);
            enemy_find(game->player->cord.y - 1, game->player->cord.x - 1, game_win);
            after_find_enemy(game_win);
        }
    }
    if(1){ //Enemy Alert
        if(is_enemy_or_not){
            wattron(game_win, COLOR_PAIR(14));
            mvwprintw(game_win, 2, 82, "                                                                                       ");
            mvwprintw(game_win, 2, 82, "  Hey, there is an enemy near you!");
            wattroff(game_win, COLOR_PAIR(14));
        }
    // if(is_enemy_or_not){
    //     mvwprintw(game_win, 2, 82, "                                                                                       ");
    //     is_enemy_or_not2 = 1 - is_enemy_or_not2;
    // }
    // if(is_enemy_or_not && is_enemy_or_not2 == 0){
    //     wattron(game_win, COLOR_PAIR(14));
    //     mvwprintw(game_win, 2, 82, "                                                                                       ");
    //     mvwprintw(game_win, 2, 82, "Hey, there is an enemy near you!");
    //     wattroff(game_win, COLOR_PAIR(14));
    //     // is_enemy_or_not2 = 1 - is_enemy_or_not2;
    // }
    }
    wattroff(game_win, COLOR_PAIR(3));
    wattroff(game_win, COLOR_PAIR(color_pair_num));
    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+' 
    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '?'
    || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'L'){
        for(int i = 0; i < game->levels[game->currentLevel]->roomsCount; i++){
            if(((game->player->cord.x == game->levels[game->currentLevel]->rooms[i]->x
            || game->player->cord.x == game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width - 1)
            &&(game->player->cord.y >= game->levels[game->currentLevel]->rooms[i]->y 
            && game->player->cord.y <= game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height))
            || ((game->player->cord.y == game->levels[game->currentLevel]->rooms[i]->y
            || game->player->cord.y == game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height - 1)
            && (game->player->cord.x >= game->levels[game->currentLevel]->rooms[i]->x 
            && game->player->cord.x <= game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width))){
                if(game->player->room_num != i){
                    // game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = 0;
                    last_attemps = 0;
                }
                in_spell = 0;
                in_nightmare = 0;
                game->player->room_num = i;
                lock_color = 7;
                if(game->levels[game->currentLevel]->rooms[i]->doors[game->levels[game->currentLevel]->rooms[i]->lock_door].is_open == false){
                    lock_color = 6;
                }
                // if(game->levels[game->currentLevel]->rooms[i]->isVisible == false || game->player->room_num == total_rooms - 1){
                    game->levels[game->currentLevel]->rooms[i]->isVisible = true;
                    draw_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                // }
                if(game->levels[game->currentLevel]->rooms[i]->type == 'm'){
                    in_nightmare = 1;
                }
                if(game->levels[game->currentLevel]->rooms[i]->type == 'p'){
                    in_spell = 1;
                }
                game->levels[game->currentLevel]->rooms[i]->isVisible = true;
                break;
                }
            }
        }
    if(game->currentLevel == game->levelCount - 1 && is_last_room == 0){
        if(game->levels[game->currentLevel]->rooms[0]->enemyLeft <= 0){
            for(int i = 0; i < game->levels[game->currentLevel]->rooms[0]->enemyCount + 1; i++){
                if(game->levels[game->currentLevel]->rooms[0]->enemies[i].type == 'i'){
                    game->levels[game->currentLevel]->rooms[0]->enemies[i].isAlive = true;
                }
            }
            // Mix_PauseMusic();
            game->levels[game->currentLevel]->rooms[0]->enemyCount++;
            draw_last_room(game_win, game->levels[game->currentLevel]->rooms[0], 0);
            is_last_room = 1;
            effects = Mix_LoadMUS("./Base_Sounds/Invader.mp3");
            Mix_PlayMusic(effects, 1);
        }
    }
    if(game->currentLevel == 6 && game->levels[game->currentLevel]->rooms[0]->enemyLeft == 0){
        is_battle = 1;
        game->currentLevel = level_save;
        after_battle = 1;
        return;
    }
    if(is_last_room){
        draw_last_room(game_win, game->levels[game->currentLevel]->rooms[0], 0);
    }
    else if(game->currentLevel == game->levelCount - 1 || game->currentLevel == 6){
        draw_room(game_win, game->levels[game->currentLevel]->rooms[0], 0);
    }
}
void enemy_be_attaked(int y, int x, WINDOW *game_win){
    if(is_enemy(y, x)){
        for(int i = 0; i < game->levels[game->currentLevel]->rooms[game->player->room_num]->enemyCount; i++){
            if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x == x 
            && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y == y){
                if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].health - game->player->currentWeapon.damage <= 0
                && game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isAlive == true){
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].health = 0;
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isAlive = false;
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].isVisible = false;
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->enemyLeft--;
                    wattron(game_win, COLOR_PAIR(color_pair_num));
                    game->player->points += game->player->currentWeapon.damage;
                    mvwprintw(game_win, 3, 82, "                                                                      ");
                    if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type == 'i'){
                        mvwprintw(game_win, 3, 82, "You dealt %d damage and killed the final boss🏆", game->player->currentWeapon.damage);
                        game->player->points += 20000;
                        game->player->gold += 1000;
                        draw_last_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], 0);
                        wrefresh(game_win);
                        sleep(2);
                        escape_check = 1;
                        win_game_screen(start_time);
                    }
                    else{
                        mvwprintw(game_win, 3, 82, "You dealt %d damage and killed %s💥", game->player->currentWeapon.damage, game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].name);
                    }
                    game->whole_map[game->currentLevel][game->player->currentWeapon.damage, game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.y]
                    [game->player->currentWeapon.damage, game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].cord.x].item = '.';
                    wattroff(game_win, COLOR_PAIR(color_pair_num));
                }
                else{
                    if(game->player->currentWeapon.type == 'w'){
                        if(game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].type != 'i'){
                            game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].canMove = false;
                        }
                    }
                    game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].health -= game->player->currentWeapon.damage;
                    game->player->points += game->player->currentWeapon.damage * 5;
                    wattron(game_win, COLOR_PAIR(color_pair_num));
                    mvwprintw(game_win, 3, 82, "                                                                          ");
                    mvwprintw(game_win, 3, 82, "You dealt %d damage to %s🔥", game->player->currentWeapon.damage, game->levels[game->currentLevel]->rooms[game->player->room_num]->enemies[i].name);
                    wattroff(game_win, COLOR_PAIR(color_pair_num));
                }
            }
        }
    }
}
void attacking(WINDOW *game_win){
    if(game->player->currentWeapon.type == 'm' || game->player->currentWeapon.type == 's'){
        if(game->player->currentWeapon.type == 's'){
            game->player->currentWeapon.damage = game->player->currentWeapon.count * 5;
        }
        else if(game->player->currentWeapon.type == 'm'){
            game->player->currentWeapon.damage = 5;
        }
        if(game->player->Power_effect > 0){
            game->player->currentWeapon.damage *= 2;
        }
        enemy_be_attaked(game->player->cord.y, game->player->cord.x, game_win);
        enemy_be_attaked(game->player->cord.y, game->player->cord.x + 1, game_win);
        enemy_be_attaked(game->player->cord.y, game->player->cord.x - 1, game_win);
        enemy_be_attaked(game->player->cord.y + 1, game->player->cord.x, game_win);
        enemy_be_attaked(game->player->cord.y - 1, game->player->cord.x, game_win);
        enemy_be_attaked(game->player->cord.y + 1, game->player->cord.x + 1, game_win);
        enemy_be_attaked(game->player->cord.y + 1, game->player->cord.x - 1, game_win);
        enemy_be_attaked(game->player->cord.y - 1, game->player->cord.x + 1, game_win);
        enemy_be_attaked(game->player->cord.y - 1, game->player->cord.x - 1, game_win);
        draw_attacked_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
    }
    else{
        if(game->player->currentWeapon.count <= 0){
            wattron(game_win, COLOR_PAIR(color_pair_num));
            mvwprintw(game_win, 3, 82, "                                                                                 ");
            mvwprintw(game_win, 3, 82, "You're out of ammo! Change your weapon...");
            wattroff(game_win, COLOR_PAIR(color_pair_num));
            return;
        }
        game->player->currentWeapon.move = 0;
        if(game->player->currentWeapon.type == 'd'){
            game->player->currentWeapon.damage = 12;
            game->player->currentWeapon.range = 5;
        }
        else if(game->player->currentWeapon.type == 'w'){
            game->player->currentWeapon.damage = 15;
            game->player->currentWeapon.range = 10;
        }
        else if(game->player->currentWeapon.type == 'a'){
            game->player->currentWeapon.damage = 5;
            game->player->currentWeapon.range = 5;
        }
        if(game->player->Power_effect > 0){
            game->player->currentWeapon.damage *= 2;
        }
        if(move_click == ' '){
            direction = wgetch(game_win);
        }
        int dx = 0;
        int dy = 0;
        if(direction == KEY_UP || direction == 'w' || direction == 'W'){
            dy = -1;
        }
        else if(direction == KEY_DOWN || direction == 's' || direction == 'S'){
            dy = 1;
        }
        else if(direction == KEY_LEFT || direction == 'a' || direction == 'A'){
            dx = -1;
        }
        else if(direction == KEY_RIGHT || direction == 'd' || direction == 'D'){
            dx = 1;
        }
        else{
            return;
        }
        int x = game->player->cord.x + dx, y = game->player->cord.y + dy;
        if(valid_move_check(y, x) 
            && game->whole_map[game->currentLevel][y][x].item != '+'
            && game->whole_map[game->currentLevel][y][x].item != '?'
            && game->whole_map[game->currentLevel][y][x].item != 'L'
            && game->whole_map[game->currentLevel][y][x].item != '#'){
                game->player->currentWeapon.count--;
                for(int i = 1; i < 4; i++){
                    if(game->player->weapons[i].type == game->player->currentWeapon.type){
                        game->player->weapons[i].count--;
                    }
                }
            }
        while(1){
            if(valid_move_check(y, x) 
            && game->whole_map[game->currentLevel][y][x].item != '+'
            && game->whole_map[game->currentLevel][y][x].item != '?'
            && game->whole_map[game->currentLevel][y][x].item != 'L'
            && game->whole_map[game->currentLevel][y][x].item != '#'
            && game->player->currentWeapon.move < game->player->currentWeapon.range){
                if(is_enemy(y, x)){
                    enemy_be_attaked(y, x, game_win);
                    draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
                    break;
                }
                else{
                    wattron(game_win, COLOR_PAIR(color_pair_num));
                    if(dx == 1){
                        mvwprintw(game_win, y, x, "→");
                    }
                    else if(dx == -1){
                        mvwprintw(game_win, y, x, "←");
                    }
                    else if(dy == 1){
                        mvwprintw(game_win, y, x, "↓");
                    }
                    else if(dy == -1){
                        mvwprintw(game_win, y, x, "↑");
                    }
                    mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "@");
                    wattroff(game_win, COLOR_PAIR(color_pair_num));
                    wrefresh(game_win);
                    if(in_nightmare){
                        draw_nightmare_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
                    }
                    else{
                        draw_ranged_attacked_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num, dx, dy);
                    }
                    usleep(15000);
                    x += dx;
                    y += dy;
                }
            }
            else{
                x -= dx;
                y -= dy;
                game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[game->levels[game->currentLevel]->rooms[game->player->room_num]->weaponCount].count = 1;
                game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[game->levels[game->currentLevel]->rooms[game->player->room_num]->weaponCount].type = game->player->currentWeapon.type;
                game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[game->levels[game->currentLevel]->rooms[game->player->room_num]->weaponCount].cord.x = x;
                game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[game->levels[game->currentLevel]->rooms[game->player->room_num]->weaponCount].cord.y = y;
                game->levels[game->currentLevel]->rooms[game->player->room_num]->weapons[game->levels[game->currentLevel]->rooms[game->player->room_num]->weaponCount].isUsed = false;
                game->levels[game->currentLevel]->rooms[game->player->room_num]->weaponCount++;
                if(game->player->currentWeapon.type == 'd'){
                    game->whole_map[game->currentLevel][y][x].item = '2';
                }
                if(game->player->currentWeapon.type == 'w'){
                    game->whole_map[game->currentLevel][y][x].item = '3';
                }
                if(game->player->currentWeapon.type == 'a'){
                    game->whole_map[game->currentLevel][y][x].item = '4';
                }
                if(in_nightmare){
                    draw_nightmare_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
                }
                else{
                    draw_ranged_attacked_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num, dx, dy);
                }
                break;
            }
            game->player->currentWeapon.move++;
        }
    }
}
void quoting(WINDOW *game_win){
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING + 5;
    int message = rand() % 31;
    mvwprintw(game_win, map_height - 10 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 9 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 8 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 7 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 6 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 5 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 4 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 3 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 2 - 1, map_width - 54 - 23, "                                                                        ");
    mvwprintw(game_win, map_height - 1 - 1, map_width - 54 - 23, "                                                                        ");
    // mvwprintw(game_win, map_height - 0 - 1, map_width - 54 - 23, "                                                                        ");
    wattron(game_win, COLOR_PAIR(color_pair_num));
    if(color_pair_num == 0){
        wattron(game_win, COLOR_PAIR(14));
    }
    mvwprintw(game_win, map_height - 8, map_width - 22, "█████████████████");
    mvwprintw(game_win, map_height - 7, map_width - 22, "█████████████████");
    mvwprintw(game_win, map_height - 6, map_width - 22, "█████████████████");
    mvwprintw(game_win, map_height - 5, map_width - 22, "███  ████  ██████");
    mvwprintw(game_win, map_height - 4, map_width - 22, "█████    ████████");
    mvwprintw(game_win, map_height - 3, map_width - 22, "█████████████████");
    wattroff(game_win, COLOR_PAIR(color_pair_num));
    wattroff(game_win, COLOR_PAIR(14));
    wattron(game_win, COLOR_PAIR(3));
    if(message == 0){
        int a = 9;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│You could live a better life, if you⧹");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│had a better mind and a better body _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰───────────────────────────────────┘");
    }
    else if(message == 1){
        int a = 12;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭───────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│You are deeply attached to your friends⧹");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│and acquaintances. Just leave it!      _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰──────────────────────────────────────┘");
    }
    else if(message == 2){
        int a = -1;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭──────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│It was all so different   │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│before everything changed._⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰─────────────────────────┘  ¯");
    }
    else if(message == 3){
        int a = 8;
        int b = 1;
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, " /───────────────────────────────────\\");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "< Excellent day to have a rotten day._⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, " \\__________________________________/  ¯");
    }
    else if(message == 4){
        int a = 14;
        int b = 0;
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│Q: What's the difference between Bell    │");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ Labs and the Boy Scouts of America?     │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│A: The Boy Scouts have adult supervision _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰────────────────────────────────────────┘  ¯");
    }
    else if(message == 5){
        int a = 13;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ Q: Why did the WASP cross the road?    │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│ A: To get to the middle. :>           _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰───────────────────────────────────────┘  ¯");
    }
    else if(message == 6){
        int a = 14;
        int b = 0;
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "╭────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│Perilous to all of us are the devices of│");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│an art deeper than we ourselves possess.│");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ -- Gandalf the Grey                    │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│ [J.R.R. Tolkien,\"Lord of the Rings\"]  _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰───────────────────────────────────────┘  ¯");
    }
    else if(message == 7){
        int a = 14;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ The human race has one really effective │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│ weapon, and that is laughter. =D        _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰────────────────────────────────────────┘  ¯");
    }
    else if(message == 8){
        int a = 14;
        int b = 1;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ Q: How do you shoot a blue elephant?    │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ A: With a blue-elephant gun.            │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│ Q: How do you shoot a pink elephant?    │");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ A: Twist its trunk until it turns blue, │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│  then shoot it with a blue-elephant gun._⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰────────────────────────────────────────┘  ¯");
    }
    else if(message == 9){
        int a = 1;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭────────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ Beware of a tall black man │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│ With one blond shoe!       _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰───────────────────────────┘  ¯");
    }
    else if(message == 10){
        int a = 10;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭─────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ Don't tell any big lies today.      │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│ Small ones can be just as effective._⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰────────────────────────────────────┘  ¯");
    }
    else if(message == 11){
        int a = 23;
        int b = 1;
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "╭──────────────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│You'll feel much better once you've given up hope.=\\");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰─────────────────────────────────────────────────┘  ¯¯");
    }
    else if(message == 12){
        int a = 9;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│You are sick, twisted and perverted.│");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│I like that in a person.           _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰───────────────────────────────────┘  ¯");
    }
    else if(message == 13){
        int a = 5;
        int b = 1;
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╭────────────────────────────────╮");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ A tall, dark and creep stranger │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│ will have more fun than you O_o _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰────────────────────────────────┘  ¯");
    }
    else if(message == 14){
        int a = 12;
        int b = 1;
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "╭───────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ Anyone who has had a bull by the tail │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│ knows five or six more things         │");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ than someone who hasn't.              │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "│  'Mark Twain'                        _⧹");
        mvwprintw(game_win, map_height - 4 - b, map_width - 54 - a, "╰─────────────────────────────────────┘  ¯");
    }
    else if(message == 15){
    int a = 13;
    int b = -1;
    mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭────────────────────────────────────────╮");
    mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ \"The only limit to our realization of  │");
    mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│  tomorrow is our doubts of today.\"     │");
    mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│ - Franklin D. Roosevelt                _⧹");
    mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "╰────────────────────────────────────────╯ ¯");
    }
    else if(message == 16){
        int a = 14;
        int b = -1;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ \"You miss 100%% of the shots you don't   │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│  take.\"                                 │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│ - Wayne Gretzky                        _⧹");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "╰────────────────────────────────────────╯ ¯");
    }
    else if(message == 17){
        int a = 14;
        int b = -1;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ \"In the end, it's not the years in      │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│  your life that count. It's the life    │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│  in your years.\"                        │");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│  \'Abraham Lincoln\'                      _\\_");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "╰────────────────────────────────────────╯");
    }
    else if(message == 18){
        int a = 14;
        int b = -1;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ \"Do not go where the path may lead,     │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│  go instead where there is no path and  │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│  leave a trail.\"                        │");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ - Ralph Waldo Emerson                   _\\_");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "╰────────────────────────────────────────╯");
    }
    else if(message == 19){
        int a = 14;
        int b = -1;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ \"Life is what happens when you're busy  │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│  making other plans.\"                   │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│ - John Lennon                           _\\");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "╰────────────────────────────────────────╯  \\");
    }
    else if(message == 20){
        int a = 14;
        int b = 0;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ Two antennas met on a roof, fell in     │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ love, and got married. The ceremony     │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│ wasn't much, but the reception was      │");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ excellent. <$_$>                        \\");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "╰─────────────────────────────────────────╯\\_");
    }
    else if(message == 21){
        int a = 14;
        int b = -2;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ I told my wife she was drawing her      │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ eyebrows too high. She looked surprised._⧹");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╰─────────────────────────────────────────╯ ¯");
    }
    else if(message == 22){
        int a = 14;
        int b = -2;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ I'm reading a book about anti-gravity.  │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ It's impossible to put down.            _⧹");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╰─────────────────────────────────────────╯ ¯");
    }
    else if(message == 23){
        int a = 14;
        int b = -2;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ Did you hear about the guy who          │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ invented Lifesavers? He made a mint!    _\\");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╰─────────────────────────────────────────╯ \\");
    }
    else if(message == 24){
        int a = 14;
        int b = -1;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ Parallel lines have so much in common.  │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ It’s a shame they’ll never meet. =_(    _\\");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╰────────────────────────────────────────╯  \\");
    }
    else if(message == 25){
        int a = 14;
        int b = -1;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ My wife told me I had to stop acting    │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ like a flamingo. So I had to put my     │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│ foot down.                             _\\");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "╰────────────────────────────────────────╯ \\_");
    }
    else if(message == 26){
        int a = 14;
        int b = -2;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ Why did the chicken go to the seance?   │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ To talk to the other side.              │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╰───────────────────────────────────────/\\_\\_");
    }
    else if(message == 27){
        int a = 14;
        int b = -2;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ I used to be addicted to the hokey      │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ pokey, but then I turned myself around. │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╰───────────────────────────────────────~\\_\\_");
    }
    else if(message == 28){
        int a = 13;
        int b = 0;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ \"The greatest glory in living lies not │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│  in never falling, but in rising every │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│  time we fall.\"                        │");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "│ - Nelson Mandela                       │");
        mvwprintw(game_win, map_height - 5 - b, map_width - 54 - a, "╰──────────────────────────────────────`\\_\\_");
    }
    else if(message == 29){
        int a = 14;
        int b = -2;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ Did you hear about the claustrophobic   │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ astronaut? He needed a little space.    │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "╰───────────────────────────────────────¯\\_\\_");
    }
    else if(message == 30){
        int a = 14;
        int b = -1;
        mvwprintw(game_win, map_height - 10 - b, map_width - 54 - a, "╭─────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 9 - b, map_width - 54 - a, "│ I told my computer I needed a break,    │");
        mvwprintw(game_win, map_height - 8 - b, map_width - 54 - a, "│ and now it won't stop sending me        │");
        mvwprintw(game_win, map_height - 7 - b, map_width - 54 - a, "│ Kit Kat ads.                            │");
        mvwprintw(game_win, map_height - 6 - b, map_width - 54 - a, "╰───────────────────────────────────────¯\\_\\_");
    }
    wattroff(game_win, COLOR_PAIR(3));
}
void messaging(WINDOW *game_win){
    if(1){ //Updating weapon
        if(game->player->currentWeapon.type == 'd'){
            game->player->currentWeapon = game->player->weapons[1];
        }
        else if(game->player->currentWeapon.type == 'w'){
            game->player->currentWeapon = game->player->weapons[2];
        }
        else if(game->player->currentWeapon.type == 'a'){
            game->player->currentWeapon = game->player->weapons[3];
        }
        else if(game->player->currentWeapon.type == 's'){
            game->player->currentWeapon = game->player->weapons[4];
        }
    }
    time(&move_time_2);
    if(difftime(move_time_2, move_time) >= 4){
        time(&move_time);
        mvwprintw(game_win, 3, 5, "                                                                                 ");
        mvwprintw(game_win, 3, 82, "                                                                                 ");
        mvwprintw(game_win, 2, 82, "                                                                                 ");
    }
    if(password_disapear % 20 == 19 || difftime(move_time_2, move_time_lock) >= 15){
        time(&move_time_lock);
        is_pass_lock = 0;
    }
    if(is_pass_lock == 0){
        mvwprintw(game_win, 2, 5, "                                                                               ");
        wattron(game_win, COLOR_PAIR(7));
        mvwprintw(game_win, 2, 5, "Current level: %d🟢 Completed level: %d✅ Total points: %lldXP", game->currentLevel + 1, game->unlockedLevel, game->player->points + game->player->gold * 10);
        wattroff(game_win, COLOR_PAIR(7));
        if(game->unlockedLevel == -1){
            mvwprintw(game_win, 2, 5, "Current level: %d🟢 Completed level: %d✅ Total points: %lld", game->currentLevel + 1, 0, game->player->points + game->player->gold * 10);
        }
        else{
            if((game->currentLevel == game->levelCount - 1 && !is_last_room) || game->currentLevel == 6){
                mvwprintw(game_win, 2, 5, "Current level: %d🟢 Completed level: %d✅ Total points: %lld Enemy Left: %d❌", game->currentLevel + 1, game->unlockedLevel, game->player->points + game->player->gold * 10, game->levels[game->currentLevel]->rooms[0]->enemyLeft);
            }
            else{
                mvwprintw(game_win, 2, 5, "Current level: %d🟢 Completed level: %d✅ Total points: %lld", game->currentLevel + 1, game->unlockedLevel, game->player->points + game->player->gold * 10);
            }
        }
    }
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING + 5;
    int effects_count = 0;
    int a = (game->player->foodCount - 1) / 100;
    int spacing = 1;
    { // Effects
        if(game->player->Power_effect > 0){
            effects_count++;
        }
        if(game->player->Health_effect > 0){
            effects_count++;
        }
        if(game->player->Speed_effect > 0){
            effects_count++;
        }
    }
    { // Cleaning
        mvwprintw(game_win, map_height - 11, 4, "                                                                                       ");
        mvwprintw(game_win, map_height - 10, 4, "                                                                                       ");
        mvwprintw(game_win, map_height - 9, 4, "                                                                                       ");
        mvwprintw(game_win, map_height - 8, 4, "                                                                                       ");
        mvwprintw(game_win, map_height - 7, 4, "                                                                                       ");
        mvwprintw(game_win, map_height - 6, 4, "                                                                                       ");
        for(int j = 0; j < 20; j++){
            mvwprintw(game_win, map_height - 6, 5 + j, " ");
            mvwprintw(game_win, map_height - 5, 5 + j, " ");
            mvwprintw(game_win, map_height - 4, 5 + j, " ");
        }
        mvwprintw(game_win, map_height - 11, 5, "                                                                                       ");
        mvwprintw(game_win, map_height - 10, 5, "                                                                                       ");
        mvwprintw(game_win, map_height - 9, 5, "                                                                                       ");
        mvwprintw(game_win, map_height - 8, 5, "                                                                                       ");
        mvwprintw(game_win, map_height - 7, 5, "                                                                                       ");
        mvwprintw(game_win, map_height - 2, 4, "                                                                                       ");
        mvwprintw(game_win, map_height - 3, 4, "                                                                                       ");
    }
    in_emoji_check();
    if(in_emoji == 0){
        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
            wattron(game_win, A_REVERSE);
        }
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "@");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
            wattroff(game_win, A_REVERSE);
        }
    }
    wattron(game_win, COLOR_PAIR(6));
    { // Spell state
        if(game->player->Speed_effect > 0){
            wattron(game_win, COLOR_PAIR(19));
            mvwprintw(game_win, map_height - 7 - effects_count, 5 + spacing, "You have Speed effect left for %d moves.🏃 ", game->player->Speed_effect);
            wattroff(game_win, COLOR_PAIR(19));
        }
        if(game->player->Power_effect > 0){
            wattron(game_win, COLOR_PAIR(17));
            int a = 0;
            if(game->player->Health_effect == 0){
                a++;
            }
            mvwprintw(game_win, map_height - 9 + a, 5 + spacing, "You have Strength effect left for %d moves.💪", game->player->Power_effect);
            wattroff(game_win, COLOR_PAIR(17));
        }
        if(game->player->Health_effect > 0){
            wattron(game_win, COLOR_PAIR(7));
            mvwprintw(game_win, map_height - 8, 5 + spacing, "You have healing effect left for %d moves.💟", game->player->Health_effect);
            wattroff(game_win, COLOR_PAIR(7));
        }
    }
    { // Weapon state
        wattron(game_win, COLOR_PAIR(14));
        if(game->player->currentWeapon.type == 'm'){
            mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Mace⚒");
        }
        else if(game->player->currentWeapon.type == 'd'){
            mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Dagger %d🗡", game->player->currentWeapon.count);
        }
        else if(game->player->currentWeapon.type == 'w'){
            mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Magic Wand %d⚚", game->player->currentWeapon.count);
        }
        else if(game->player->currentWeapon.type == 'a'){
            mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Bow and Arrow🏹 %d➳", game->player->currentWeapon.count);
        }
        else if(game->player->currentWeapon.type == 's'){
            if(game->player->currentWeapon.count == 1){
                mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Sword⚔ I");
            }
            if(game->player->currentWeapon.count == 2){
                mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Sword⚔ II");
            }
            if(game->player->currentWeapon.count == 3){
                mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Sword⚔ III");
            }
            if(game->player->currentWeapon.count == 4){
                mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Sword⚔ IV");
            }
            if(game->player->currentWeapon.count == 5){
                mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon: Sword⚔ V");
            }
        }
        wattroff(game_win, COLOR_PAIR(14));
        wattron(game_win, COLOR_PAIR(3));
        mvwprintw(game_win, map_height - 7, 5 + spacing, "Current weapon:");
        wattroff(game_win, COLOR_PAIR(3));
    }
    { // Health and Shield state
        int a = 0;
        if(in_spell){
            for(int j = 0; j < game->player->health / 10; j++){
                wchar_t heart = L'❤';
                mvwprintw(game_win, map_height - 6, 5 + spacing + 2*j, "💜");
            }
            if((game->player->health + 5) % 10 == 0){
                mvwprintw(game_win, map_height - 6, 4 + spacing + 2*game->player->health / 10, "🍆");
            }
        }
        else if(game->levels[game->currentLevel]->rooms[game->player->room_num]->type == 's'){
            for(int j = 0; j < game->player->health / 10; j++){
                wchar_t heart = L'❤';
                mvwprintw(game_win, map_height - 6, 5 + spacing + 2*j, "💚");
            }
            if((game->player->health + 5) % 10 == 0){
                mvwprintw(game_win, map_height - 6, 4 + spacing + 2*game->player->health / 10, "🥒");
            }
        }
        else{
            wattron(game_win, COLOR_PAIR(6));
            for(int j = 0; j < game->player->health / 10; j++){
                wchar_t heart = L'❤';
                mvwprintw(game_win, map_height - 6, 5 + spacing + 2*j, "❤");
            }
            if((game->player->health + 5) % 10 == 0){
                a = 2;
                mvwprintw(game_win, map_height - 6, 4 + spacing + 2*game->player->health / 10, "💔");
            }
        }
        wattroff(game_win, COLOR_PAIR(6));
        wattron(game_win, COLOR_PAIR(19));
        for(int j = 0; j < game->player->shield / 10; j++){
            mvwprintw(game_win, map_height - 6, 5 + 2*game->player->health / 10 + spacing + 2*j + a, "🌌");
        }
        if((game->player->shield + 5) % 10 == 0){
            mvwprintw(game_win, map_height - 6, 4 + spacing + 2*game->player->shield / 10 + 2*game->player->health / 10 + a, "🌀");
        }
        wattroff(game_win, COLOR_PAIR(19));
    }
    { // Food and Gold state
        for(int j = 0; j < game->player->foodCount / 10; j++){
            mvwprintw(game_win, map_height - 5 + j / 10, 5 + spacing + 2*j - 2*(j/10)*10, "🍖");
        }
        if((game->player->foodCount + 5) % 10 == 0){
            mvwprintw(game_win,  map_height - 5 + game->player->foodCount / 100, 4 + spacing + 2*game->player->foodCount / 10 - 2*(game->player->foodCount / 100)*10, "🥓");
        }
        if(poisonous_food_check == 1){
            mvwprintw(game_win,  map_height - 5 + game->player->foodCount / 100, 5 + spacing + 2*game->player->foodCount / 10 - 2*(game->player->foodCount / 100)*10, "🥩");
            poisonous_food_check = 0;
        }
        wattron(game_win, COLOR_PAIR(12));
        mvwprintw(game_win, map_height - 4 + a, 4 + spacing, " 💰%d$", game->player->gold);
        wattroff(game_win, COLOR_PAIR(12));
    }
    { //Borders
        wattron(game_win, COLOR_PAIR(15));
        mvwprintw(game_win, 1, 4, "┌───────Messages────────────────────────────────────────────────────────────┐");
        mvwprintw(game_win, 2, 80, "│");
        mvwprintw(game_win, 3, 80, "│");
        mvwprintw(game_win, 2, 4, "│");
        mvwprintw(game_win, 3, 4, "│");
        mvwprintw(game_win, 4, 4, "└───────────────────────────────────────────────────────────────────────────┘");
        mvwprintw(game_win, 1, 81, "┌───────Combat-Report───────────────────────────────────────────────────────┐");
        mvwprintw(game_win, 2, 80 + 77, "│");
        mvwprintw(game_win, 3, 80 + 77, "│");
        mvwprintw(game_win, 2, 81, "│");
        mvwprintw(game_win, 3, 81, "│");
        mvwprintw(game_win, 4, 81, "└───────────────────────────────────────────────────────────────────────────┘");
        mvwprintw(game_win, map_height - 8 - effects_count, 4, "┌───────Bar─────────────────────────────────────────────────────────────────┐");
        mvwprintw(game_win, map_height - 7 - effects_count, 80, "│");
        mvwprintw(game_win, map_height - 6 - effects_count, 80, "│");
        mvwprintw(game_win, map_height - 5 - effects_count, 80, "│");
        mvwprintw(game_win, map_height - 4 - effects_count, 80, "│");
        mvwprintw(game_win, map_height - 7 - effects_count, 4, "│");
        mvwprintw(game_win, map_height - 6 - effects_count, 4, "│");
        mvwprintw(game_win, map_height - 5 - effects_count, 4, "│");
        mvwprintw(game_win, map_height - 4 - effects_count, 4, "│");
        mvwprintw(game_win, map_height - 7, 80, "│");
        mvwprintw(game_win, map_height - 6, 80, "│");
        mvwprintw(game_win, map_height - 5, 80, "│");
        mvwprintw(game_win, map_height - 4, 80, "│");
        mvwprintw(game_win, map_height - 3, 80, "│");
        mvwprintw(game_win, map_height - 7, 4, "│");
        mvwprintw(game_win, map_height - 6, 4, "│");
        mvwprintw(game_win, map_height - 5, 4, "│");
        mvwprintw(game_win, map_height - 4, 4, "│");
        mvwprintw(game_win, map_height - 3, 4, "│");
        mvwprintw(game_win, map_height - 3 + a, 4, "└───────────────────────────────────────────────────────────────────────────┘");
        mvwhline(game_win, start_y - 1, start_x - 1, ACS_HLINE, map_width - 11);
        mvwhline(game_win, end_y, start_x - 1, ACS_HLINE, map_width - 11);
        mvwprintw(game_win, start_y - 1, start_x - 2, "╭");
        mvwprintw(game_win, start_y - 1, map_width - 6, "╮");
        mvwprintw(game_win, end_y, start_x - 2, "╰");
        mvwprintw(game_win, end_y, map_width - 6, "╯");
        for(int i = start_y; i < end_y; i++){
            mvwprintw(game_win, i, start_x - 2, "│");
            mvwprintw(game_win, i, map_width - 6, "│");
        }
        wattroff(game_win, COLOR_PAIR(15));
        time_t time_to_quote;
        time(&time_to_quote);
        if(difftime(time_to_quote, quote_time2) >= 10){
            time(&quote_time2);
            mvwprintw(game_win, map_height - 10 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 9 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 8 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 7 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 6 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 5 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 4 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 3 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 2 - 1, map_width - 54 - 23, "                                                                        ");
            mvwprintw(game_win, map_height - 1 - 1, map_width - 54 - 23, "                                                                        ");
            wattron(game_win, COLOR_PAIR(color_pair_num));
            if(color_pair_num == 0){
                wattron(game_win, COLOR_PAIR(14));
            }
            mvwprintw(game_win, map_height - 8, map_width - 22, "█████████████████");
            mvwprintw(game_win, map_height - 7, map_width - 22, "█████████████████");
            mvwprintw(game_win, map_height - 6, map_width - 22, "█████████████████");
            mvwprintw(game_win, map_height - 5, map_width - 22, "███  ████  ██████");
            mvwprintw(game_win, map_height - 4, map_width - 22, "█████    ████████");
            mvwprintw(game_win, map_height - 3, map_width - 22, "█████████████████");
            wattroff(game_win, COLOR_PAIR(color_pair_num));
            wattroff(game_win, COLOR_PAIR(14));
        }
        if(difftime(time_to_quote, quote_time) >= 20){
            time(&quote_time);
            quoting(game_win);
        }
    }
}
void inventory_bar_menu() {
    WINDOW *inventory_bar_win;
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    // int max_height = game->player->spellCount > game->player->weaponCount ? game->player->spellCount : game->player->weaponCount;
    int max_height = 3;
    inventory_bar_win = newwin(20 + max_height, 34 + 12, map_height / 2 - 14, map_width / 2 - 17);
    keypad(inventory_bar_win, TRUE);
    box(inventory_bar_win, 0, 0);

    char *inventory_items[] = {
        "Health",
        "Food",
        "Gold",
        "Weapons",
        "Spells",
        "AncientKey"
    };
    int inventory_size = sizeof(inventory_items) / sizeof(inventory_items[0]);

    char *options[] = {
        "Consume Food🌱 ",
        "Choose Weapon🏹 ",
        "Use Spell🍷",
        "Forge broken Ancientkey⚠ "
    };
    int options_size = sizeof(options) / sizeof(options[0]);

    int selected_option = 0;
    int selected_item = 0;
    int items_size = 0;
    int ch;

    while (1) {
        werase(inventory_bar_win);
        wattron(inventory_bar_win, COLOR_PAIR(color_pair_num));
        box(inventory_bar_win, 0, 0);
        wattroff(inventory_bar_win, COLOR_PAIR(color_pair_num));
        wattron(inventory_bar_win, COLOR_PAIR(3));
        mvwprintw(inventory_bar_win, 1, 1, "%s: %d💗 Shield: %d💙", inventory_items[0], game->player->health, game->player->shield);
        mvwprintw(inventory_bar_win, 2, 1, "%s: %d🍓", inventory_items[1], game->player->foodCount);
        mvwprintw(inventory_bar_win, 3, 1, "%s: %d📀", inventory_items[2], game->player->gold);
        mvwprintw(inventory_bar_win, 4, 1, "%s: ", inventory_items[3]);
        int k = 11;
        for(int i = 0; i < 5; i++){
            if(i == 0){
                mvwprintw(inventory_bar_win, 4, k, "Mace %d⚒", game->player->weapons[i].count);
                k += 9;
            }
            else if(i == 1){
                mvwprintw(inventory_bar_win, 4, k, "Dagger %d🗡", game->player->weapons[i].count);
            }
            else if(i == 2){
                mvwprintw(inventory_bar_win, 5, k, "Magic Wand %d⚚", game->player->weapons[i].count);
                k += 15;
            }
            else if(i == 3){
                mvwprintw(inventory_bar_win, 5, k, "Normal Arrow %d➳", game->player->weapons[i].count);
                k += 17;
            }
            else if(i == 4){
                mvwprintw(inventory_bar_win, 5, k, "Sword %d⚔", game->player->weapons[i].count);
            }
            int a = 0;
            if(game->player->weapons[i].count >= 10){
                a = 1;
            }
            k += a;
            if(i == 1){
                k = 1;
            }
        }
        // mvwprintw(inventory_bar_win, 4, k - 2, "  🔫");
        k = 9;
        mvwprintw(inventory_bar_win, 6, 1, "%s: ", inventory_items[4]);
        for(int i = 0; i < 3; i++){
            if(i == 0){
                mvwprintw(inventory_bar_win, 6, k, "Speed %d🍾", game->player->spells[i].count);
            }
            else if(i == 1){
                mvwprintw(inventory_bar_win, 6, k, "Power %d🍷", game->player->spells[i].count);
            }
            else if(i == 2){
                mvwprintw(inventory_bar_win, 6, k, "Healing %d🍸", game->player->spells[i].count);
            }
            int a = 0;
            if(game->player->spells[i].count >= 10){
                a = 1;
            }
            k += 10 + a; 
        }
        // mvwprintw(inventory_bar_win, 5, k - 1, "  🥛");
        mvwprintw(inventory_bar_win, 7, 1, "Ancient Key: %d", game->player->acientKey);
        mvwprintw(inventory_bar_win, 8, 1, "Broken Ancient Key: %d", game->player->brokenAcientKey);
        wattroff(inventory_bar_win, COLOR_PAIR(3));
        wattron(inventory_bar_win, COLOR_PAIR(12));
        int a = 0;
        if(game->player->brokenAcientKey >= 10){
            a = 1;
        }
        mvwprintw(inventory_bar_win, 7, 15 + a, "Δ");
        a = 0;
        if(game->player->brokenAcientKey >= 10){
            a = 1;
        }
        mvwprintw(inventory_bar_win, 8, 22 + a, "δ");
        wattroff(inventory_bar_win, COLOR_PAIR(12));

        for (int i = 0; i < options_size; i++) {
            if (i == 0) wattron(inventory_bar_win, COLOR_PAIR(7));
            if (i == 1) wattron(inventory_bar_win, COLOR_PAIR(19));
            if (i == 2) wattron(inventory_bar_win, COLOR_PAIR(17));
            if (i == 3) wattron(inventory_bar_win, COLOR_PAIR(12));
            
            if (i == selected_option) {
                wattron(inventory_bar_win, A_REVERSE);
            }
            mvwprintw(inventory_bar_win, i + inventory_size + 4, 1, "%s", options[i]);
            wattroff(inventory_bar_win, A_REVERSE);

            if (i == 0) wattroff(inventory_bar_win, COLOR_PAIR(7));
            if (i == 1) wattroff(inventory_bar_win, COLOR_PAIR(19));
            if (i == 2) wattroff(inventory_bar_win, COLOR_PAIR(17));
            if (i == 3) wattroff(inventory_bar_win, COLOR_PAIR(12));
        }

        wrefresh(inventory_bar_win);
        ch = wgetch(inventory_bar_win);

        switch (ch) {
            case KEY_UP:
                selected_option = (selected_option - 1 + options_size) % options_size;
                break;
            case KEY_DOWN:
                selected_option = (selected_option + 1) % options_size;
                break;
            case '\t':
                delwin(inventory_bar_win);
                return;
            case '\n':
                // werase(inventory_bar_win);
                wattron(inventory_bar_win, COLOR_PAIR(color_pair_num));
                box(inventory_bar_win, 0, 0);
                wattroff(inventory_bar_win, COLOR_PAIR(color_pair_num));
                if(selected_option == 0){
                    mvwprintw(inventory_bar_win, inventory_size + options_size + 4, 1, "Choose your food:");
                    items_size = 4;
                    int tab_check = 0;
                    while(tab_check != 1){
                        wattron(inventory_bar_win, COLOR_PAIR(7));
                        for(int i = 0; i < 4; i++){
                            if (i == selected_item) {
                                wattron(inventory_bar_win, A_REVERSE);
                            }
                            if(i == 0){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Special Food %d🍓 ", game->player->specialfoods[i].count);
                            }
                            if(i == 1){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Power Food %d🍘 ", game->player->specialfoods[i].count);
                            }
                            if(i == 2){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Speed Food %d🍟 ", game->player->specialfoods[i].count);
                            }
                            if(i == 3){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Enchant spell with Rotten Food %d☣💱 ", game->player->specialfoods[i].count);
                            }
                            wattroff(inventory_bar_win, A_REVERSE);
                        }
                        wrefresh(inventory_bar_win);
                        ch = wgetch(inventory_bar_win);
                        switch (ch) {
                            case KEY_UP:
                                selected_item = (selected_item - 1 + items_size) % items_size;
                                break;
                            case KEY_DOWN:
                                selected_item = (selected_item + 1) % items_size;
                                break;
                            case '\n':
                                if(selected_item == 0){
                                    if(game->player->specialfoods[0].count <= 0){
                                        wattroff(inventory_bar_win, COLOR_PAIR(7));
                                        wattron(inventory_bar_win, COLOR_PAIR(3));
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "Your Hunger is %d.   ", game->player->foodCount);
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 10, 2, "And you don't have any Special Food :b");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 11, 2, "You need to eat more!   ");
                                        wattroff(inventory_bar_win, COLOR_PAIR(3));
                                        wattron(inventory_bar_win, COLOR_PAIR(7));
                                    }
                                    else if(game->player->health > 80){
                                        wattroff(inventory_bar_win, COLOR_PAIR(7));
                                        wattron(inventory_bar_win, COLOR_PAIR(3));
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "Your Health is %d.", game->player->health);
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 10, 2, "And it can't be more than 100 :b   ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 11, 2, "Get back to the field!   ");
                                        wattroff(inventory_bar_win, COLOR_PAIR(3));
                                        wattron(inventory_bar_win, COLOR_PAIR(7));
                                    }
                                    else{
                                        game->player->health += 20;
                                        game->player->specialfoods[0].count--;
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "You gained 20 health");
                                        wattron(inventory_bar_win, COLOR_PAIR(3));
                                        mvwprintw(inventory_bar_win, 1, 1, "%s: %d💗", inventory_items[0], game->player->health);
                                        mvwprintw(inventory_bar_win, 2, 1, "%s: %d🍓", inventory_items[1], game->player->foodCount);
                                        wattroff(inventory_bar_win, COLOR_PAIR(3));
                                    }
                                }
                            else if(selected_item == 1){
                                if(game->player->specialfoods[1].count <= 0){
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "Not enough foods!");
                                }
                                else{
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "You used a Power Food.");
                                    game->player->specialfoods[1].count--;
                                    game->player->Power_effect += 10;
                                }
                            }
                            else if(selected_item == 2){
                                if(game->player->specialfoods[2].count <= 0){
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "Not enough foods!");
                                }
                                else{
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "You used a Speed Food.");
                                    game->player->specialfoods[2].count--;
                                    game->player->Speed_effect += 10;
                                }
                            }
                            else if(selected_item == 3){
                                if(game->player->specialfoods[3].count <= 1){
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "Not enough Rotten foods!");
                                }
                                else{
                                    if(rand() % 2){
                                        if(rand() % 2){
                                            mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "You've enchanted a Speed Spell!");
                                            game->player->spells[0].count++;
                                        }
                                        else{
                                            mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "You've enchanted a Power Spell!");
                                            game->player->spells[1].count++;
                                        }
                                    }
                                    else{
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "Ah, bad luck, hope you get it next time =(");
                                    }
                                    game->player->specialfoods[3].count -= 2;
                                }
                            }
                                break;
                            case '\t':
                                tab_check = 1;
                                break;
                        }
                        wattroff(inventory_bar_win, COLOR_PAIR(7));
                    }
                    selected_item = 0;
                }
                else if(selected_option == 1){
                    mvwprintw(inventory_bar_win, inventory_size + options_size + 4, 1, "Choose your weapon type:");
                    char *weapon_types[] = {"Melee Weapon", "Ranged Weapon"};
                    int weapon_types_size = 2;
                    int selected_type = 0;
                    int tab_check1 = 0;
                    while (tab_check1 != 1) {
                        mvwprintw(inventory_bar_win, inventory_size + options_size + 5, 1, "                                           ");
                        mvwprintw(inventory_bar_win, inventory_size + options_size + 7, 2, "                                       ");
                        // mvwprintw(inventory_bar_win, inventory_size + options_size + 6, 1, "                                           ");
                        // mvwprintw(inventory_bar_win, inventory_size + options_size + 7, 2, "                                       ");
                        // mvwprintw(inventory_bar_win, inventory_size + options_size + 8, 2, "                              ");
                        // mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "                              ");
                        // mvwprintw(inventory_bar_win, inventory_size + options_size + 10, 2, "                              ");
                        // mvwprintw(inventory_bar_win, inventory_size + options_size + 11, 2, "                              ");
                        wattron(inventory_bar_win, COLOR_PAIR(19));
                        for (int i = 0; i < weapon_types_size; i++) {
                            if (i == selected_type) {
                                wattron(inventory_bar_win, A_REVERSE);
                            }
                            if(i == 0){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5, 1, "%s🪓 ", weapon_types[i]);
                            }
                            else if(i == 1){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5, 1 + i * 29, "%s➳ ", weapon_types[i]);
                            }
                            wattroff(inventory_bar_win, A_REVERSE);
                        }
                        wrefresh(inventory_bar_win);
                        ch = wgetch(inventory_bar_win);

                        switch (ch) {
                            case KEY_RIGHT:
                                selected_type = (selected_type - 1 + weapon_types_size) % weapon_types_size;
                                break;
                            case KEY_LEFT:
                                selected_type = (selected_type + 1) % weapon_types_size;
                                break;
                            case '\n':
                                if (selected_type == 0) { // Melee Weapon
                                    char *melee_weapons[] = {"Mace", "Sword"};
                                    int items_size = 2;
                                    int selected_item = 0;
                                    int tab_check = 0;
                                    while (tab_check != 1) {
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 5, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 6, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 7, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 8, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 10, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 11, 1, "                                            ");
                                        for (int i = 0; i < items_size; i++) {
                                            if (i == selected_item) {
                                                wattron(inventory_bar_win, A_REVERSE);
                                            }
                                            if(i == 0){
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Mace⚒ Damage: 5");
                                            }
                                            else if(i == 1){
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Sword⚔ Sharpness: %d Damage: %d", game->player->weapons[4].count, game->player->weapons[4].count * 5);
                                            }
                                            wattroff(inventory_bar_win, A_REVERSE);
                                        }
                                        wrefresh(inventory_bar_win);
                                        ch = wgetch(inventory_bar_win);

                                        switch (ch) {
                                            case KEY_UP:
                                                selected_item = (selected_item - 1 + items_size) % items_size;
                                                break;
                                            case KEY_DOWN:
                                                selected_item = (selected_item + 1) % items_size;
                                                break;
                                            case '\n':
                                                wattroff(inventory_bar_win, COLOR_PAIR(19));
                                                wattron(inventory_bar_win, COLOR_PAIR(3));
                                                if (game->player->weapons[4].count <= 0 && selected_item == 1) {
                                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 4 + items_size, 1, "                                            ");
                                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 4 + items_size, 2, "You don't have that weapon!                ");
                                                } else {
                                                    if(selected_item == 0){
                                                        game->player->currentWeapon = game->player->weapons[0];
                                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 4 + items_size, 1, "                                            ");
                                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 4 + items_size, 2, "You choosed %s                    ", game->player->weapons[0].name);
                                                    }
                                                    if(selected_item == 1){
                                                        game->player->currentWeapon = game->player->weapons[4];
                                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 4 + items_size, 2, "                                            ");
                                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 4 + items_size, 2, "You choosed %s                  ", game->player->weapons[4].name);
                                                    }
                                                }
                                                wattroff(inventory_bar_win, COLOR_PAIR(3));
                                                wattron(inventory_bar_win, COLOR_PAIR(19));
                                                tab_check = 1;
                                                break;
                                            case '\t':
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5, 1, "                                           ");
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 6, 2, "                                       ");
                                                tab_check = 1;
                                                break;
                                        }
                                    }
                                } else if (selected_type == 1) { // Ranged Weapon
                                    char *ranged_weapons[] = {"Dagger", "Magic Wand", "Normal Arrow"};
                                    int items_size = 3;
                                    int selected_item = 0;
                                    int tab_check = 0;
                                    while (tab_check != 1) {
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 5, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 6, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 7, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 8, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 10, 1, "                                            ");
                                        mvwprintw(inventory_bar_win, inventory_size + options_size + 11, 1, "                                            ");
                                        for (int i = 0; i < items_size; i++) {
                                            if (i == selected_item) {
                                                wattron(inventory_bar_win, A_REVERSE);
                                            }
                                            if(i == 0){
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Dagger🗡 Damage: 12 Range: 5 Count: %d", game->player->weapons[i + 1].count);
                                            }
                                            else if(i == 1){
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● MagicWand⚚Damage: 15 Range: 10 Count: %d", game->player->weapons[i + 1].count);
                                            }
                                            else if(i == 2){
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Bow🏹 Damage: 5 Range: 5 Count: %d", game->player->weapons[i + 1].count);
                                            }
                                            wattroff(inventory_bar_win, A_REVERSE);
                                        }
                                        wrefresh(inventory_bar_win);
                                        ch = wgetch(inventory_bar_win);

                                        switch (ch) {
                                            case KEY_UP:
                                                selected_item = (selected_item - 1 + items_size) % items_size;
                                                break;
                                            case KEY_DOWN:
                                                selected_item = (selected_item + 1) % items_size;
                                                break;
                                            case '\n':
                                                wattroff(inventory_bar_win, COLOR_PAIR(19));
                                                wattron(inventory_bar_win, COLOR_PAIR(3));
                                                if (game->player->weapons[selected_item + 1].count <= 0) {
                                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 3 + items_size, 1, "                                            ");
                                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 3 + items_size, 2, "You don't have that projectile!         ");
                                                } else {
                                                    game->player->currentWeapon = game->player->weapons[selected_item + 1];
                                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 3 + items_size, 2, "                                           ");
                                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 3 + items_size, 2, "You choosed %s              ", game->player->weapons[selected_item + 1].name);
                                                }
                                                tab_check = 1;
                                                wattroff(inventory_bar_win, COLOR_PAIR(3));
                                                wattron(inventory_bar_win, COLOR_PAIR(19));
                                                break;
                                            case '\t':
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5, 1, "                                           ");
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 6, 2, "                                           ");
                                                mvwprintw(inventory_bar_win, inventory_size + options_size + 7, 2, "                                       ");
                                                tab_check = 1;
                                                break;
                                        }
                                    }
                                }
                                break;
                            case '\t':
                                tab_check1 = 1;
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 7, 2, "                                       ");
                                break;
                        }
                        wattroff(inventory_bar_win, COLOR_PAIR(19));
                    }
                }
                else if(selected_option == 2){
                    mvwprintw(inventory_bar_win, inventory_size + options_size + 4, 1, "Choose your spell:");
                    items_size = 3;
                    int tab_check = 0;
                    while(tab_check != 1){
                        wattron(inventory_bar_win, COLOR_PAIR(17));
                        for(int i = 0; i < items_size; i++){
                            if (i == selected_item) {
                                wattron(inventory_bar_win, A_REVERSE);
                            }
                            if(i == 0){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● %s🍾 %d", game->player->spells[i].name, game->player->spells[i].count);
                            }
                            if(i == 1){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● %s🍷 %d", game->player->spells[i].name, game->player->spells[i].count);
                            }
                            if(i == 2){
                                mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● %s🍸 %d", game->player->spells[i].name, game->player->spells[i].count);
                            }
                            wattroff(inventory_bar_win, A_REVERSE);
                        }
                        wrefresh(inventory_bar_win);
                        ch = wgetch(inventory_bar_win);
                        mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + items_size, 2, "                                    ");
                        switch (ch) {
                            case KEY_UP:
                                selected_item = (selected_item - 1 + items_size) % items_size;
                                break;
                            case KEY_DOWN:
                                selected_item = (selected_item + 1) % items_size;
                                break;
                            case '\n':
                                wattroff(inventory_bar_win, COLOR_PAIR(17));
                                wattron(inventory_bar_win, COLOR_PAIR(3));
                                if(game->player->spells[selected_item].count <= 0){
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + items_size, 2, "                                    ");
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + items_size, 2, "Not enough spells");  
                                    wattroff(inventory_bar_win, COLOR_PAIR(3));
                                    wattron(inventory_bar_win, COLOR_PAIR(17));
                                    break;
                                }
                                else{
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + items_size, 2, "                                    ");
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + items_size, 2, "You used %s", game->player->spells[selected_item].name);
                                    game->player->spells[selected_item].count--;
                                    game->player->spellCount--;
                                    if(game->player->spells[selected_item].type == 's'){
                                        game->player->Speed_effect += 15;
                                    }
                                    if(game->player->spells[selected_item].type == 'p'){
                                        game->player->Power_effect += 15;
                                    }
                                    if(game->player->spells[selected_item].type == 'h'){
                                        game->player->Health_effect += 15;
                                    }
                                    k = 9;
                                    for(int i = 0; i < 3; i++){
                                        if(i == 0){
                                            mvwprintw(inventory_bar_win, 6, k, "Speed %d🍾", game->player->spells[i].count);
                                        }
                                        else if(i == 1){
                                            mvwprintw(inventory_bar_win, 6, k, "Power %d🍷", game->player->spells[i].count);
                                        }
                                        else if(i == 2){
                                            mvwprintw(inventory_bar_win, 6, k, "Healing %d🍸", game->player->spells[i].count);
                                        }
                                        int a = 0;
                                        if(game->player->spells[i].count >= 10){
                                            a = 1;
                                        }
                                        k += 10 + a; 
                                    }
                                    wattroff(inventory_bar_win, COLOR_PAIR(3));
                                    wattron(inventory_bar_win, COLOR_PAIR(17));
                                    break;
                                }
                            case '\t':
                                tab_check = 1;
                                break;
                        }
                        wattroff(inventory_bar_win, COLOR_PAIR(17));
                    }
                    selected_item = 0;
                }
                else if(selected_option == 3){
                    mvwprintw(inventory_bar_win, inventory_size + options_size + 4, 1, "You have %d broken Ancientkeys:  ", game->player->brokenAcientKey);
                    items_size = 1;
                    int tab_check = 0;
                    while(tab_check != 1){
                        mvwprintw(inventory_bar_win, inventory_size + options_size + 4, 1, "You have %d broken Ancientkeys:  ", game->player->brokenAcientKey);
                        wattron(inventory_bar_win, COLOR_PAIR(12));
                        for(int i = 0; i < 1; i++){
                            if (i == selected_item) {
                                wattron(inventory_bar_win, A_REVERSE);
                            }
                            mvwprintw(inventory_bar_win, inventory_size + options_size + 5 + i, 2, "● Forge");
                            wattroff(inventory_bar_win, A_REVERSE);
                        }
                        wrefresh(inventory_bar_win);
                        ch = wgetch(inventory_bar_win);
                        switch (ch) {
                            case KEY_UP:
                                selected_item = (selected_item - 1 + items_size) % items_size;
                                break;
                            case KEY_DOWN:
                                selected_item = (selected_item + 1) % items_size;
                                break;
                            case '\n':
                                if(game->player->brokenAcientKey <= 1){
                                    wattroff(inventory_bar_win, COLOR_PAIR(12));
                                    wattron(inventory_bar_win, COLOR_PAIR(3));
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 7, 2, "Not enough broken Ancientkeys.  ");
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 8, 2, "                              ");
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "                              ");
                                    wattroff(inventory_bar_win, COLOR_PAIR(3));
                                    wattron(inventory_bar_win, COLOR_PAIR(12));
                                }
                                else{
                                    wattroff(inventory_bar_win, COLOR_PAIR(12));
                                    wattron(inventory_bar_win, COLOR_PAIR(3));
                                    game->player->acientKey++;
                                    game->player->brokenAcientKey -= 2;
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 7, 2, "Forged Successfully.     ");
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 8, 2, "                        ");
                                    mvwprintw(inventory_bar_win, inventory_size + options_size + 9, 2, "                    ");
                                    wattroff(inventory_bar_win, COLOR_PAIR(3));
                                    wattron(inventory_bar_win, COLOR_PAIR(12));
                                }
                                wattron(inventory_bar_win, COLOR_PAIR(3));
                                mvwprintw(inventory_bar_win, 7, 1, "Ancient Key: %d", game->player->acientKey);
                                mvwprintw(inventory_bar_win, 8, 1, "Broken Ancient Key: %d", game->player->brokenAcientKey);
                                wattroff(inventory_bar_win, COLOR_PAIR(3));
                                break;
                            case '\t':
                                tab_check = 1;
                                break;
                        }
                        wattroff(inventory_bar_win, COLOR_PAIR(12));
                    }
                    selected_item = 0;
                }
                // mvwprintw(inventory_bar_win, inventory_size + options_size + 2, 1, "Option Under %s:", options[selected_option]);
                // mvwprintw(inventory_bar_win, inventory_size + options_size + 3, 1, " [Sample Choice 1]");
                // mvwprintw(inventory_bar_win, inventory_size + options_size + 4, 1, " [Sample Choice 2]");
                wrefresh(inventory_bar_win);
                // wgetch(inventory_bar_win);
                break;
        }
    }
    werase(inventory_bar_win);
    wrefresh(inventory_bar_win);
    delwin(inventory_bar_win);
}
void x_print(WINDOW *game_win, int x, int y){
    if(game->whole_map[game->currentLevel][y][x].item == '^'){
        wattron(game_win, COLOR_PAIR(10));
        mvwprintw(game_win, y, x, "^");
        wattroff(game_win, COLOR_PAIR(10));
    }
    else if(game->whole_map[game->currentLevel][y][x].item == '?'){
        wattron(game_win, COLOR_PAIR(10));
        mvwprintw(game_win, y, x, "‽");
        wattroff(game_win, COLOR_PAIR(10));
    }
}
void player_move(WINDOW *game_win, time_t start_time){
    time(&move_time);
    time(&move_time_2);
    time(&move_time_lock);
    time(&quote_time);
    count_move = 1;
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING + 5;
    game->map_height = map_height;
    game->map_width = map_width;
    poisonous_food_check = 0;
    int window_check = 0;
    direction = 'w';
    g_check = 0;
    f_check = 0;
    x_check = 0;
    m_check = 0;
    time(&move_time);
    time(&rotten_time);
    time(&quote_time2);
    { //First printings.
        wattron(game_win, COLOR_PAIR(15));
        mvwprintw(game_win, 1, 4, "┌───────Messages────────────────────────────────────────────────────────────┐");
        mvwprintw(game_win, 2, 4, "│                                                                           │");
        mvwprintw(game_win, 3, 4, "│                                                                           │");
        mvwprintw(game_win, 4, 4, "└───────────────────────────────────────────────────────────────────────────┘");
        mvwprintw(game_win, 1, 81, "┌───────Combat-Report───────────────────────────────────────────────────────┐");
        mvwprintw(game_win, 2, 81, "│                                                                           │");
        mvwprintw(game_win, 3, 81, "│                                                                           │");
        mvwprintw(game_win, 4, 81, "└───────────────────────────────────────────────────────────────────────────┘");
        mvwprintw(game_win, map_height - 11, 4, "┌───────Bar─────────────────────────────────────────────────────────────────┐");
        mvwprintw(game_win, map_height - 10, 4, "│                                                                           │");
        mvwprintw(game_win, map_height - 9, 4, "│                                                                           │");
        mvwprintw(game_win, map_height - 8, 4, "│                                                                           │");
        mvwprintw(game_win, map_height - 7, 4, "│                                                                           │");
        mvwprintw(game_win, map_height - 6, 4, "│                                                                           │");
        mvwprintw(game_win, map_height - 5, 4, "│                                                                           │");
        mvwprintw(game_win, map_height - 4, 4, "│                                                                           │");
        mvwprintw(game_win, map_height - 3, 4, "│                                                                           │");
        mvwprintw(game_win, map_height - 2, 4, "└───────────────────────────────────────────────────────────────────────────┘");
        mvwhline(game_win, start_y - 1, start_x - 1, ACS_HLINE, map_width - 11);
        mvwhline(game_win, end_y, start_x - 1, ACS_HLINE, map_width - 11);
        mvwprintw(game_win, start_y - 1, start_x - 2, "╭");
        mvwprintw(game_win, start_y - 1, map_width - 6, "╮");
        mvwprintw(game_win, end_y, start_x - 2, "╰");
        mvwprintw(game_win, end_y, map_width - 6, "╯");
        for(int i = start_y; i < end_y; i++){
            mvwprintw(game_win, i, start_x - 2, "│");
            mvwprintw(game_win, i, map_width - 6, "│");
        }
        wattroff(game_win, COLOR_PAIR(15));
        if(color_pair_num == 0){
            wattron(game_win, COLOR_PAIR(14));
        }
        else{
            wattron(game_win, COLOR_PAIR(color_pair_num));
        }
        mvwprintw(game_win, map_height - 8, map_width - 22, "█████████████████");
        mvwprintw(game_win, map_height - 7, map_width - 22, "█████████████████");
        mvwprintw(game_win, map_height - 6, map_width - 22, "█████████████████");
        mvwprintw(game_win, map_height - 5, map_width - 22, "███  ████  ██████");
        mvwprintw(game_win, map_height - 4, map_width - 22, "█████    ████████");
        mvwprintw(game_win, map_height - 3, map_width - 22, "█████████████████");
        if(color_pair_num == 0){
            wattroff(game_win, COLOR_PAIR(14));
        }
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, map_height - 11, map_width - 77, "╭──────────────────────────────────────────────────╮");
        mvwprintw(game_win, map_height - 10, map_width - 77, "│ Hi, welcome to AID Rouge Dungeon🎆🎇             │");
        mvwprintw(game_win, map_height - 9, map_width - 77, "│ My name is Steve and I'm Arad's best friend :D   │");
        mvwprintw(game_win, map_height - 8, map_width - 77, "│ I'm your guide through this adventurous journey. │");
        mvwprintw(game_win, map_height - 7, map_width - 77, "│ And I'll entertain you with my quotes =)         │");
        mvwprintw(game_win, map_height - 6, map_width - 77, "│ Important messages are show with your color!     │");
        mvwprintw(game_win, map_height - 5, map_width - 77, "│ Hope you enjoy this game and also survive it😃  _⧹");
        mvwprintw(game_win, map_height - 4, map_width - 77, "╰─────────────────────────────────────────────────┘  ¯");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
        wattron(game_win, COLOR_PAIR(7));
        mvwprintw(game_win, 2, 5, "Current level: %d🟢 Completed level: %d✅ Total points: %lldXP", game->currentLevel + 1, game->unlockedLevel + 1, game->player->points);
        wattroff(game_win, COLOR_PAIR(7));
        mvwprintw(game_win, 2, 5, "Current level: %d🟢 Completed level: %d✅ Total points: %lld", game->currentLevel + 1, game->unlockedLevel + 1, game->player->points);
    }
    while(1){
        if(in_spell){
            wattron(game_win, COLOR_PAIR(17));
        }
        else if(game->currentLevel == game->levelCount - 1){
            wattron(game_win, COLOR_PAIR(12));
        }
        else if(game->currentLevel == 6){
            wattron(game_win, COLOR_PAIR(14));
        }
        { // Rottening
            time(&rotten_time_check);
            if(difftime(rotten_time_check, rotten_time) >= 30){
                time(&rotten_time);
                if(game->player->specialfoods[0].count > 0){
                    game->player->specialfoods[0].count--;
                    game->player->specialfoods[3].count++;
                }
                if(game->player->specialfoods[1].count > 0){
                    game->player->specialfoods[1].count--;
                    game->player->specialfoods[3].count++;
                }
                if(game->player->specialfoods[2].count > 0){
                    game->player->specialfoods[2].count--;
                    game->player->specialfoods[3].count++;
                }
            }
        }
        in_emoji = 0;
        diff_from_map = 0;
        move_or_not = 0;
        is_enemy_or_not = 0;
        // wattron(game_win, COLOR_PAIR(3));
        // // mvwprintw(game_win, 2, 5, "Current level: %d Completed level: %d", game->currentLevel + 1, game->unlockedLevel + 1);
        // wattroff(game_win, COLOR_PAIR(3));
        if(is_last_room){
            draw_last_room(game_win, game->levels[game->currentLevel]->rooms[0], 0);
        }
        else if(game->currentLevel == game->levelCount - 1 || game->currentLevel == 6){
            draw_room(game_win, game->levels[game->currentLevel]->rooms[0], 0);
        }
        move_click = wgetch(game_win);
        if(is_last_room){
            draw_last_room(game_win, game->levels[game->currentLevel]->rooms[0], 0);
        }
        else if(game->currentLevel == game->levelCount - 1 || game->currentLevel == 6){
            draw_room(game_win, game->levels[game->currentLevel]->rooms[0], 0);
        }
        if(move_click == 'f' || move_click == 'F'){
            move_click = wgetch(game_win);
            f_check = 1 - f_check;
        }
        else if(move_click == 'g' || move_click == 'G'){ // if(password_disapear % 20 == 19){
        //     mvwprintw(game_win, 2, 2, "                                                                      ");
        // }
            g_check = 1 - g_check;
        }
        else if(move_click == 'x' || move_click == 'X'){
            x_check = 1 - x_check;
        }
        else if(move_click == 'm' || move_click == 'M'){
            m_check = 1 - m_check;
            if(m_check){
                print_all_map(game_win);
            }
            else{
                print_map(game_win);
            }
        }
        else if(move_click == 't' || move_click == 'T'){
            t_check = 1 - t_check;
            if(m_check){
                print_all_map(game_win);
            }
            else{
                print_map(game_win);
            }
        }
        if(x_check){
            x_print(game_win, game->player->cord.x + 1, game->player->cord.y);
            x_print(game_win, game->player->cord.x - 1, game->player->cord.y);
            x_print(game_win, game->player->cord.x, game->player->cord.y + 1);
            x_print(game_win, game->player->cord.x, game->player->cord.y - 1);
            x_print(game_win, game->player->cord.x + 1, game->player->cord.y + 1);
            x_print(game_win, game->player->cord.x - 1, game->player->cord.y + 1);
            x_print(game_win, game->player->cord.x + 1, game->player->cord.y - 1);
            x_print(game_win, game->player->cord.x - 1, game->player->cord.y - 1);
        }
        // if(count_move_click % 5 == 0){
        //     mvwprintw(game_win, map_height - 7, 5, "                                                              ");
        //     mvwprintw(game_win, map_height - 5 + (game->player->foodCount) / 100, 5 + 2*(game->player->foodCount / 10) - 2*((game->player->foodCount)/100)*10, "  ");
        // }
        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
            wattron(game_win, COLOR_PAIR(3));
        }
        if(move_click == 27){
            wclear(game_win);
            wrefresh(game_win);
            strcpy(which_pause_menu, " ");
            pause_menu();
            if(!strcmp(which_pause_menu, " Save and Quit ")){
                Mix_HaltMusic();
                escape_check = 1;
                game->player->can_be_saved = 1;
                return;
            }
            else if(!strcmp(which_pause_menu, " Pause Music ")){
                Mix_PauseMusic();
            }
            else if(!strcmp(which_pause_menu, " Resume Music ")){
                Mix_ResumeMusic();
            }
            else if(!strcmp(which_pause_menu, " Select Music ")){
                Mix_PauseMusic();
                selected_music = display_music_menu();
                play_music(selected_music);
            }
            if(m_check){
                print_all_map(game_win);
            }
            else{
                print_map(game_win);
            }
        }
        if(game->levels[game->currentLevel]->rooms[game->player->room_num]->type == 's'){
            wattron(game_win, COLOR_PAIR(15));
        }
        if(window_check){
            clear_room(game_win, game->levels[game->currentLevel]->rooms[window_check - 1], window_check - 1);
            window_check = 0;
        }
        else if(move_click == KEY_UP || move_click == 'w' || move_click == 'W'){
            if(valid_move_check(game->player->cord.y - 1,game->player->cord.x)){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
                printf_diff_from_map(game_win);
                game->player->cord.y -= 1;
                count_move++;
                password_disapear++;
                move_or_not = 1;
            }
            else if(game->whole_map[game->currentLevel][game->player->cord.y - 1][game->player->cord.x].item == '=' && m_check == 0){
                window_check = 0;
                for(int j = game->player->cord.y - 2; j > start_y + 1; j--){
                    for(int k = game->player->cord.x - 10; k < game->player->cord.x + 10; k++){
                        if(j >= start_x && j <= end_x){
                            for(int i = 0; i < game->levels[game->currentLevel]->roomsCount; i++){
                                if((((k == game->levels[game->currentLevel]->rooms[i]->x
                                || k == game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width - 1)
                                &&(j >= game->levels[game->currentLevel]->rooms[i]->y 
                                && j <= game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height))
                                || ((j == game->levels[game->currentLevel]->rooms[i]->y
                                || j == game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height - 1)
                                && (k >= game->levels[game->currentLevel]->rooms[i]->x 
                                && k <= game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width)))
                                && game->levels[game->currentLevel]->rooms[i]->isVisible == false){
                                    draw_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                                    if(game->levels[game->currentLevel]->rooms[i]->isVisible)
                                    game->levels[game->currentLevel]->rooms[i]->isVisible = false;
                                    // clear_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                                    window_check = i + 1;
                                    break;
                                }
                            }
                        }
                        if(window_check){
                            break;
                        }
                    }
                    if(window_check){
                        // sleep(1);
                        break;
                    }
                }
            }
            else if(game->whole_map[game->currentLevel][game->player->cord.y - 1][game->player->cord.x].item == 'L'){
                // last_attemps = game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps;
                entering_password();
                game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = last_attemps;
                mvwprintw(game_win, 3, 2, "                                                                      ");
                if(m_check){
                    print_all_map(game_win);
                }
                else{
                    print_map(game_win);
                }
            }
            int speed_check = 0;
            // if(game->player->Speed_effect > 0){
            //     speed_check = 1;
            // }
            while((f_check || speed_check) && valid_move_check(game->player->cord.y - 1,game->player->cord.x)){
                game->player->cord.y -= 1;
                speed_check = 0;
                move_or_not = 1;
                if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
                    continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 3);
                }
                else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+'
                || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '?'
                || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'L'){
                    break;
                }
            }
        }
        else if(move_click == KEY_DOWN || move_click == 's' || move_click == 'S'){
            if(valid_move_check(game->player->cord.y + 1,game->player->cord.x)){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
                printf_diff_from_map(game_win);
                game->player->cord.y += 1;
                count_move++;
                password_disapear++;
                move_or_not = 1;
            }
            else if(game->whole_map[game->currentLevel][game->player->cord.y + 1][game->player->cord.x].item == '=' && m_check == 0){
                window_check = 0;
                for(int j = game->player->cord.y + 2; j < end_y; j++){
                    for(int k = game->player->cord.x - 15; k < game->player->cord.x + 15; k++){
                        if(j >= start_x && j <= end_x){
                            for(int i = 0; i < game->levels[game->currentLevel]->roomsCount; i++){
                                if((((k == game->levels[game->currentLevel]->rooms[i]->x
                                || k == game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width - 1)
                                &&(j >= game->levels[game->currentLevel]->rooms[i]->y 
                                && j <= game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height))
                                || ((j == game->levels[game->currentLevel]->rooms[i]->y
                                || j == game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height - 1)
                                && (k >= game->levels[game->currentLevel]->rooms[i]->x 
                                && k <= game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width)))
                                && game->levels[game->currentLevel]->rooms[i]->isVisible == false){
                                    draw_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                                    game->levels[game->currentLevel]->rooms[i]->isVisible = false;
                                    // clear_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                                    window_check = i + 1;
                                    break;
                                }
                            }
                        }
                        if(window_check){
                            break;
                        }
                    }
                    if(window_check){
                        // sleep(1);
                        break;
                    }
                }
            }
            else if(game->whole_map[game->currentLevel][game->player->cord.y + 1][game->player->cord.x].item == 'L'){
                // last_attemps = game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps;
                entering_password();
                game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = last_attemps;
                mvwprintw(game_win, 3, 2, "                                                                      ");
                if(m_check){
                    print_all_map(game_win);
                }
                else{
                    print_map(game_win);
                }
            }
            int speed_check = 0;
            // if(game->player->Speed_effect > 0){
            //     speed_check = 1;
            // }
            while((f_check || speed_check) && valid_move_check(game->player->cord.y + 1,game->player->cord.x)){
                game->player->cord.y += 1;
                speed_check = 0;
                move_or_not = 1;
                if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
                    continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 3);
                }
                else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+'
                || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '?'
                || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'L'){
                    break;
                }
            }
        }
        else if(move_click == KEY_RIGHT || move_click == 'd' || move_click == 'D'){
            if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '?' && game->player->room_num == game->levels[game->currentLevel]->secret_room){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "‽");
                game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->doorCount - 1].password = 1;
                game->player->room_num = game->levels[game->currentLevel]->roomsCount - 1;
                game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[0].password = 1;
                game->player->cord.x = game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[0].cord.x;
                game->player->cord.y = game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[0].cord.y;
            }
            else if(valid_move_check(game->player->cord.y,game->player->cord.x + 1)){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
                printf_diff_from_map(game_win);
                game->player->cord.x += 1;
                count_move++;
                password_disapear++;
                move_or_not = 1;
            }
            else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == '=' && m_check == 0){
                window_check = 0;
                for(int k = game->player->cord.x + 2; k < end_x - 4; k++){
                    for(int j = game->player->cord.y - 15; j < game->player->cord.y + 15; j++){
                        if(j >= start_y && j <= end_y){
                            for(int i = 0; i < game->levels[game->currentLevel]->roomsCount; i++){
                                if((((k == game->levels[game->currentLevel]->rooms[i]->x
                                || k == game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width - 1)
                                &&(j >= game->levels[game->currentLevel]->rooms[i]->y 
                                && j <= game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height))
                                || ((j == game->levels[game->currentLevel]->rooms[i]->y
                                || j == game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height - 1)
                                && (k >= game->levels[game->currentLevel]->rooms[i]->x 
                                && k <= game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width)))
                                && game->levels[game->currentLevel]->rooms[i]->isVisible == false){
                                    draw_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                                    game->levels[game->currentLevel]->rooms[i]->isVisible = false;
                                    // clear_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                                    window_check = i + 1;
                                    break;
                                }
                            }
                        }
                        if(window_check){
                            break;
                        }
                    }
                    if(window_check){
                        // sleep(1);
                        break;
                    }
                }
            }
            else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x + 1].item == 'L'){
                // game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = 1;
                // last_attemps = game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps;
                entering_password();
                game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = last_attemps;
                mvwprintw(game_win, 3, 2, "                                                                      ");
                if(m_check){
                    print_all_map(game_win);
                }
                else{
                    print_map(game_win);
                }
            }
            int speed_check = 0;
            // if(game->player->Speed_effect > 0){
            //     speed_check = 1;
            // }
            while((f_check || speed_check) && valid_move_check(game->player->cord.y,game->player->cord.x + 1)){
                game->player->cord.x += 1;
                speed_check = 0;
                if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
                    continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 3);
                }
                else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+' 
                || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '?'
                || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'L'){
                    if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '?'){
                        game->player->cord.x -= 1;
                    }
                    break;
                }
            }
        }
        else if(move_click == KEY_LEFT || move_click == 'a' || move_click == 'A'){
            if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '?' && game->player->room_num == total_rooms - 1){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "‽");
                game->player->room_num = game->levels[game->currentLevel]->secret_room;
                game->player->cord.x = game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->doorCount - 1].cord.x;
                game->player->cord.y = game->levels[game->currentLevel]->rooms[game->player->room_num]->doors[game->levels[game->currentLevel]->rooms[game->player->room_num]->doorCount - 1].cord.y;
            }
            else if(valid_move_check(game->player->cord.y,game->player->cord.x - 1)){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
                printf_diff_from_map(game_win);
                game->player->cord.x -= 1;
                count_move++;
                password_disapear++;
                move_or_not = 1;
            }
            else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == '=' && m_check == 0){
                window_check = 0;
                for(int k = game->player->cord.x - 1; k < start_x + 4; k++){
                    for(int j = game->player->cord.y - 15; j > game->player->cord.y + 15; j--){
                        if(j >= start_y && j <= end_y){
                            for(int i = 0; i < game->levels[game->currentLevel]->roomsCount; i++){
                                if((((k == game->levels[game->currentLevel]->rooms[i]->x
                                || k == game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width - 1)
                                &&(j >= game->levels[game->currentLevel]->rooms[i]->y 
                                && j <= game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height))
                                || ((j == game->levels[game->currentLevel]->rooms[i]->y
                                || j == game->levels[game->currentLevel]->rooms[i]->y + game->levels[game->currentLevel]->rooms[i]->height - 1)
                                && (k >= game->levels[game->currentLevel]->rooms[i]->x 
                                && k <= game->levels[game->currentLevel]->rooms[i]->x + game->levels[game->currentLevel]->rooms[i]->width)))
                                && game->levels[game->currentLevel]->rooms[i]->isVisible == false){
                                    draw_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                                    game->levels[game->currentLevel]->rooms[i]->isVisible = false;
                                    // clear_room(game_win, game->levels[game->currentLevel]->rooms[i], i);
                                    window_check = i + 1;
                                    break;
                                }
                            }
                        }
                        if(window_check){
                            break;
                        }
                    }
                    if(window_check){
                        // sleep(1);
                        break;
                    }
                }
            }
            else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x - 1].item == 'L'){
                // last_attemps = game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps;
                entering_password();
                game->levels[game->currentLevel]->rooms[game->player->room_num]->lock_attemps = last_attemps;
            mvwprintw(game_win, 3, 2, "                                                                      ");
                if(m_check){
                    print_all_map(game_win);
                }
                else{
                    print_map(game_win);
                }
            }
            int speed_check = 0;
            // if(game->player->Speed_effect > 0){
            //     speed_check = 1;
            // }
            while((f_check || speed_check) && valid_move_check(game->player->cord.y,game->player->cord.x - 1)){
                game->player->cord.x -= 1;
                speed_check = 0;
                if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
                    continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 3);
                }
                else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+'
                || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '?'
                || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == 'L'){
                    break;
                }
            }
        }
        else if(move_click == KEY_HOME || move_click == 'q' || move_click == 'Q'){
            if(valid_move_check(game->player->cord.y - 1,game->player->cord.x - 1)){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
                printf_diff_from_map(game_win);
                game->player->cord.x -= 1;
                game->player->cord.y -= 1;
                count_move++;
                password_disapear++;
                move_or_not = 1;
            }
            while(f_check && valid_move_check(game->player->cord.y - 1,game->player->cord.x - 1)){
                game->player->cord.y -= 1;
                game->player->cord.x -= 1;
                if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
                    continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 3);
                }
                else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+'){
                    break;
                }
            }
        }
        else if(move_click == KEY_PPAGE){
            if(valid_move_check(game->player->cord.y - 1,game->player->cord.x + 1)){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
                printf_diff_from_map(game_win);
                game->player->cord.x += 1;
                game->player->cord.y -= 1;
                count_move++;
                password_disapear++;
                move_or_not = 1;
            }
            while(f_check && valid_move_check(game->player->cord.y - 1,game->player->cord.x + 1)){
                game->player->cord.y -= 1;
                game->player->cord.x += 1;
                if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
                    continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 3);
                }
                else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+'){
                    break;
                }
            }
        }
        else if(move_click == KEY_NPAGE || move_click == 'c' || move_click == 'C'){
            if(valid_move_check(game->player->cord.y + 1,game->player->cord.x + 1)){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
                printf_diff_from_map(game_win);
                game->player->cord.x += 1;
                game->player->cord.y += 1;
                count_move++;
                password_disapear++;
                move_or_not = 1;
            }
            while(f_check && valid_move_check(game->player->cord.y + 1,game->player->cord.x + 1)){
                game->player->cord.y += 1;
                game->player->cord.x += 1;
                if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
                    continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 3);
                }
                else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+'){
                    break;
                }
            }
        }
        else if(move_click == KEY_END || move_click == 'z' || move_click == 'Z'){
            if(valid_move_check(game->player->cord.y + 1,game->player->cord.x - 1)){
                mvwprintw(game_win, game->player->cord.y, game->player->cord.x, "%lc", game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item);
                printf_diff_from_map(game_win);
                game->player->cord.x -= 1;
                game->player->cord.y += 1;
                count_move++;
                password_disapear++;
                move_or_not = 1;
            }
            while(f_check && valid_move_check(game->player->cord.y + 1,game->player->cord.x - 1)){
                game->player->cord.y += 1;
                game->player->cord.x -= 1;
                if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
                    continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 3);
                }
                else if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+'){
                    break;
                }
            }
        }
        else if(move_click == '>' || move_click == '.'){
            if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '>'){
                game->currentLevel++;
                if(game->unlockedLevel < game->currentLevel){
                    game->unlockedLevel = game->currentLevel;
                    unlocked_level = game->unlockedLevel;
                    game->newLevel = 1;
                }
                current_level = game->currentLevel;
                back_front = 1;
                return;
            }
        }
        else if(move_click == '<' || move_click == ','){
            if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '<'){
                game->currentLevel--;
                current_level = game->currentLevel;
                back_front = -1;
                return;
            }
        }
        else if(move_click == 'P' || move_click == 'p'){
            move_click = wgetch(game_win);
            if(move_click == '>' || move_click == '.'){
                game->currentLevel++;
                if(game->unlockedLevel < game->currentLevel){
                    game->unlockedLevel = game->currentLevel;
                    unlocked_level = game->unlockedLevel;
                    game->newLevel = 1;
                }
                current_level = game->currentLevel;
                back_front = 1;
                return;
            }
            else if(move_click == '<' || move_click == ','){
                game->currentLevel--;
                current_level = game->currentLevel;
                back_front = -1;
                return;
            }
        }
        else if(move_click == ' ' || move_click == 'e' || move_click == 'E'){
            move_or_not = 1;
            attacking(game_win);
            count_move++;
        }
        else if(move_click == '\t'){
            inventory_bar_menu();
            if(m_check){
                print_all_map(game_win);
            }
            else{
                print_map(game_win);
            }
        }
        after_move(game_win);
        if(is_battle){
            is_battle = 0;
            if(!after_battle){
                game->newLevel = 1;
            }
            return;
        }
        if(game->levels[game->currentLevel]->rooms[game->player->room_num]->type == 's'){
            wattroff(game_win, COLOR_PAIR(15));
        }
        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '+' || game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
            continue_corridor(game_win, game->player->cord.y, game->player->cord.x, 0);
        }
        if(game->levels[game->currentLevel]->rooms[game->player->room_num]->type == 'm'){
            in_nightmare = 1;
        }
        if(game->whole_map[game->currentLevel][game->player->cord.y][game->player->cord.x].item == '#'){
            in_spell = 0;
        }
        if(game->levels[game->currentLevel]->rooms[game->player->room_num]->type == 's' && count_move % 16 == 2 && game->player->health <= 95 && move_or_not){
            game->player->health += 5;
        }
        if(in_spell && move_or_not){
            if(count_move % (100 / (difficulty_efficiency * 2 + 1)) == 0){
                game->player->health -= 5;
                // count_move = 1;
            }
        }
        else if(move_or_not){
            int a = 1;
            if(game->player->Speed_effect > 0){
                a = 2;
            }
            if(count_move % (150 / (difficulty_efficiency * 2 + 1) / a) == 0 && game->player->foodCount == 0){
                game->player->health -= 5;
                // count_move = 1;
            }
            else if(count_move % (90 / (difficulty_efficiency * 2 + 1) / a) == 0 && game->player->foodCount > 0){
                game->player->foodCount -= 5;
                // count_move = 1;
            }
        }
        int is_healing = 1;
        if(game->player->Health_effect > 0){
            is_healing = 5;
        }
        int a = 1;
        if(game->player->Speed_effect > 0){
            a = 2;
        }
        if(count_move % (25 / is_healing / a) == 0 && (game->player->foodCount > 30 || game->player->Health_effect > 0) && game->player->health <= 95 && in_spell == 0 
        && game->levels[game->currentLevel]->rooms[game->player->room_num]->type != 's' && move_or_not){
            game->player->health += 5;
            // count_move = 1;
        }
        // if(count_move % 20 == 2 && game->player->foodCount > 30 && game->player->health <= 90 && in_spell == 0 
        // && game->levels[game->currentLevel]->rooms[game->player->room_num]->type != 's'){
        //     game->player->foodCount -= 5;
        // }
        if(in_nightmare){
            draw_nightmare_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
        }
        else if(in_spell){
            draw_room(game_win, game->levels[game->currentLevel]->rooms[game->player->room_num], game->player->room_num);
        }
        if(in_spell){
            wattroff(game_win, COLOR_PAIR(17));
        }
        else if(game->currentLevel == game->levelCount - 1){
            wattroff(game_win, COLOR_PAIR(12));
        }
        else if(game->currentLevel == 6){
            wattroff(game_win, COLOR_PAIR(14));
        }
        messaging(game_win);
        if(game->player->health <= 0){
            lost_game_screen(start_time);
            escape_check = 1;
            game->new_game = true;
            return;
        }
        if(count_move >= 1000){
            count_move = 1;
        }
        wrefresh(game_win);
    }
}
void game_play(WINDOW *game_win){
    if(game->currentLevel == game->levelCount - 1){
        if(rand() % 2 == 0){
            effects = Mix_LoadMUS("./Base_Sounds/PiratesoftheCaribbean.mp3");
        }
        else{
            effects = Mix_LoadMUS("./Base_Sounds/GameOfThrones.mp3");
        }
        Mix_PlayMusic(effects, 1);
    }
    if(game->newLevel){
        generate_map();
        game->newLevel = 0;
    }
    current_level = game->currentLevel;
    unlocked_level = game->unlockedLevel;
    if(after_battle){
        after_battle = 0;
        game->player->cord.x = x_save;
        game->player->cord.y = y_save;
    }
    else if(back_front == 1){
        if(game->currentLevel == game->levelCount - 1 || game->currentLevel == 6){
            game->player->cord.x = game->levels[game->currentLevel]->rooms[0]->x + 1;
            game->player->cord.y = game->levels[game->currentLevel]->rooms[0]->y + 1;
        }
        else{
            game->player->cord.x = game->levels[game->currentLevel]->rooms[0]->stair.cord.x;
            game->player->cord.y = game->levels[game->currentLevel]->rooms[0]->stair.cord.y;
        }
        game->player->room_num = 0;
    }
    else if(back_front == -1){
        int a = 0;
        if(game->levels[game->currentLevel]->is_secret_room == true){
            a = -1;
        } 
        game->player->cord.x = game->levels[game->currentLevel]->rooms[game->levels[game->currentLevel]->roomsCount - 1 + a]->stair.cord.x;
        game->player->cord.y = game->levels[game->currentLevel]->rooms[game->levels[game->currentLevel]->roomsCount - 1 + a]->stair.cord.y;
        game->player->room_num = game->levels[game->currentLevel]->roomsCount - 1 + a;
    }
    print_map(game_win);
    player_move(game_win, start_time);
    in_spell = 0;
    in_nightmare = 0;
    all_print = 0;
    if(escape_check){
        return;
    }
    game_play(game_win);
}
void start_game(){
    // strcpy(which_pause_menu, " Save and Quit ");
    // pause_menu();
    time(&start_time);
    effects = Mix_LoadMUS("./Base_Sounds/Eating.mp3");
    player = (Player *)malloc(sizeof(Player));
    game = (Game *)malloc(sizeof(Game));
    game->player = (Player *)malloc(sizeof(Player));
    game->levelCount = 4 + rand() % 3;
    game->levels = (Level **)malloc(7 * sizeof(Level *));
    game->currentLevel = 0;
    game->unlockedLevel = 0;
    game->newLevel = 1;
    unlocked_level = -1;
    // game->levelCount = 2;
    game->player->Speed_effect = 0;
    game->player->Health_effect = 0;
    game->player->Power_effect = 0;
    game->player->can_be_saved = 1;
    Room rooms[9][9];
    for (int i = 0; i < 7; i++)
    {
        game->levels[i] = (Level *)malloc(sizeof(Level));
        game->levels[i]->rooms = malloc(9 * sizeof(Room*));
        game->levels[i]->level = i;
        game->levels[i]->key.x = 0;
        game->levels[i]->key.y = 0;
        for(int j = 0; j < 9; j++){
            game->levels[i]->rooms[j] = malloc(sizeof(Room));
            game->levels[i]->rooms[j] = (rooms[i] + j);
            // game->levels[i]->rooms[j]->doors = malloc(12 * sizeof(Door));
            // game->levels[i]->rooms[j]->pillars = malloc(5 * sizeof(Point));
        }
    }
    WINDOW *game_win;
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING;
    srand(time(NULL));
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    game_win = newwin(map_height, map_width, 0, 0);
    strcpy(game->player->color, which_color);
    { //Jsut a shit
    // if(game->currentLevel != 0){
    //     for(int i = 0; i < rooms[0].height; i++){
    //         int k = 0;
    //         for(int j = 0; j < rooms[0].width; j++){
    //             if(game->whole_map[game->currentLevel][i][j].item == '.'){
    //                 k = 1;
    //                 game->player->cord.x = rooms[0].x + i;
    //                 game->player->cord.y = rooms[0].y + j;
    //                 break;
    //             }
    //         }
    //         if(k){
    //             break;
    //         }
    //     }
    // }
    // struct timeval tv;
    // gettimeofday(&tv, NULL);
    // milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    // // u = user;
    // // printf("%d , %d\n", maxX, maxY);
    // noecho();
    // damageTime -= (user->setting.level - 1) * 5;
    // time_t a = time(NULL);
    // initilizing the game
    // game->currentLevel = currentLevel;

    // initialize player
    // game->player->cord.x = game->levels[game->currentLevel]->rooms[0]->cord.x + 2;
    // game->player->cord.y = game->levels[game->currentLevel]->rooms[0]->cord.y + 2;
    // game->player->state = 1;
    // game->player->room = game->levels[game->currentLevel]->rooms[0];
    }
    if(new_game_check == 0){
        // generate_map();
        game->player->health = 100;
        game->player->shield = 0;
        game->player->foodCount = current_user.food;
        game->player->weaponCount = current_user.weaponsCount;
        game->player->spellCount = current_user.spellsCount;
        game->player->weapons = (Weapon *)malloc(5 * sizeof(Weapon));
        game->player->weapons[0].count = current_user.weaponsCount_each[0]; game->player->weapons[0].type = 'm'; strcpy(game->player->weapons[0].name, "Mace"); game->player->weapons[0].damage = 5;
        game->player->weapons[1].count = current_user.weaponsCount_each[1]; game->player->weapons[1].type = 'd'; strcpy(game->player->weapons[1].name, "Dagger"); game->player->weapons[1].damage = 12;
        game->player->weapons[2].count = current_user.weaponsCount_each[2]; game->player->weapons[2].type = 'w'; strcpy(game->player->weapons[2].name, "MagicWand"); game->player->weapons[2].damage = 15;
        game->player->weapons[3].count = current_user.weaponsCount_each[3]; game->player->weapons[3].type = 'a'; strcpy(game->player->weapons[3].name, "NormalArrow"); game->player->weapons[3].damage = 5;
        game->player->weapons[4].count = current_user.weaponsCount_each[4]; game->player->weapons[4].type = 's'; strcpy(game->player->weapons[4].name, "Sword"); game->player->weapons[4].damage = 10;
        game->player->spells = (Spell *)malloc(3 * sizeof(Spell));
        game->player->spells[0].count = current_user.spellsCount_each[0]; game->player->spells[0].type = 's'; strcpy(game->player->spells[0].name, "Speed");
        game->player->spells[1].count = current_user.spellsCount_each[1]; game->player->spells[1].type = 'p'; strcpy(game->player->spells[1].name, "Power");
        game->player->spells[2].count = current_user.spellsCount_each[2]; game->player->spells[2].type = 'h'; strcpy(game->player->spells[2].name, "Healing");
        game->player->specialfoods = (Food *)malloc(4 * sizeof(Food));
        game->player->specialfoods[0].count = 0; game->player->specialfoods[0].type = 'n';
        game->player->specialfoods[1].count = 0; game->player->specialfoods[1].type = 'p';
        game->player->specialfoods[2].count = 0; game->player->specialfoods[2].type = 's';
        game->player->specialfoods[3].count = 0; game->player->specialfoods[3].type = 'r';
        game->player->currentWeapon = game->player->weapons[0];
        // for(int i = 0; i < 5; i++){
        //     strcpy(game->player->weapons[i].name, current_user.weapons[i]);
        // }
        // for(int i = 0; i < 3; i++){
        //     strcpy(game->player->spells[i].name, current_user.spells[i]);
        // }
        game->player->level = current_user.level;
        game->player->acientKey = current_user.ancientkey;
        game->player->brokenAcientKey = current_user.brokenancientkey;
        game->player->gold = current_user.golds;
        game->player->name = current_user.username;
        game->player->room_num = 0;
        game->player->points = current_user.points;
        game->player->games_count = current_user.games_count;
        game->player->play_time = current_user.play_time;
        game->new_game = false;
        load_user_game();
    }
    else{
        game->new_game = false;
        game->player->health = 100;
        game->player->foodCount = 100;
        game->player->shield = 0;
        game->player->weaponCount = 0;
        game->player->spellCount = 0;
        game->player->weapons = (Weapon *)malloc(5 * sizeof(Weapon));
        game->player->weapons[0].count = 1; game->player->weapons[0].type = 'm'; strcpy(game->player->weapons[0].name, "Mace"); game->player->weapons[0].damage = 5;
        game->player->weapons[1].count = 0; game->player->weapons[1].type = 'd'; strcpy(game->player->weapons[1].name, "Dagger"); game->player->weapons[1].damage = 12;
        game->player->weapons[2].count = 0; game->player->weapons[2].type = 'w'; strcpy(game->player->weapons[2].name, "MagicWand"); game->player->weapons[2].damage = 15;
        game->player->weapons[3].count = 0; game->player->weapons[3].type = 'a'; strcpy(game->player->weapons[3].name, "NormalArrow"); game->player->weapons[3].damage = 5;
        game->player->weapons[4].count = 0; game->player->weapons[4].type = 's'; strcpy(game->player->weapons[4].name, "Sword"); game->player->weapons[4].damage = 10;
        game->player->spells = (Spell *)malloc(3 * sizeof(Spell));
        game->player->spells[0].count = 0; game->player->spells[0].type = 's'; strcpy(game->player->spells[0].name, "Speed");
        game->player->spells[1].count = 0; game->player->spells[1].type = 'p'; strcpy(game->player->spells[1].name, "Power");
        game->player->spells[2].count = 0; game->player->spells[2].type = 'h'; strcpy(game->player->spells[2].name, "Healing");
        game->player->specialfoods = (Food *)malloc(4 * sizeof(Food));
        game->player->specialfoods[0].count = 0; game->player->specialfoods[0].type = 'n';
        game->player->specialfoods[1].count = 0; game->player->specialfoods[1].type = 'p';
        game->player->specialfoods[2].count = 0; game->player->specialfoods[2].type = 's';
        game->player->specialfoods[3].count = 0; game->player->specialfoods[3].type = 'r';
        game->player->currentWeapon = game->player->weapons[0];
        game->player->level = 0;
        game->currentLevel = 0;
        game->player->acientKey = 0;
        game->player->brokenAcientKey = 0;
        game->player->gold = current_user.golds;
        game->player->acientKey = current_user.ancientkey;
        game->player->brokenAcientKey = current_user.brokenancientkey;
        game->player->name = current_user.username;
        game->player->room_num = 0;
        game->player->points = current_user.points;
        game->player->games_count = current_user.games_count;
        game->player->play_time = current_user.play_time;
    }
    new_game_check = 0;
    game->player->can_be_saved = 1;
    // game->player = player;
    // game->levels = levels;
    keypad(game_win, TRUE);
    clear();
    game_play(game_win);
    if(escape_check){
        delwin(game_win);
        time_t end_time;
        time(&end_time);
        game->player->play_time += difftime(end_time, start_time);
        game->player->points = game->player->play_time * 5 + game->player->gold * 10;
        return;
    }
    wclear(game_win); 
    werase(game_win);
    wrefresh(game_win); 
    delwin(game_win);
    // getch();
}