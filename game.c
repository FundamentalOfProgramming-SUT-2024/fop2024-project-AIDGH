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

Room rooms[10];
Level *levels[6];
Map *whole_map[20000];
char whole_map2[6][200][200];
Corridor *corridors[50];
Player *player;
extern User current_user;
extern int total_rooms;
extern int current_level;
extern int unlocked_level;
extern char which_color[50];
extern int new_game_check;
int color_pair_num;
int escape_check = 0;
int difficulty_efficiency;
void draw_room(WINDOW *game_win, Room *room, int room_number) {
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
    for(int i = 0; i < room->pillarcount; i++){
        mvwprintw(game_win, room->pillars[i].y, room->pillars[i].x, "▒");
    }
    for(int i = 0; i < room->foodCount; i++){
        if(room->foods[i].isUsed == false){
            mvwprintw(game_win, room->foods[i].cord.y, room->foods[i].cord.x, "%%");
        }
    }
    for(int i = 0; i < room->goldCount; i++){
        if(room->golds[i].isUsed == false){
            mvwprintw(game_win, room->golds[i].cord.y, room->golds[i].cord.x, "$");
        }
    }
    for(int i = 0; i < room->trapCount; i++){
        if(room->traps[i].isVisible == true){
            mvwprintw(game_win, room->traps[i].cord.y, room->traps[i].cord.x, "^");
        }
    }
    for(int i = 0; i < room->doorCount; i++){
        mvwprintw(game_win, room->doors[i].cord.y, room->doors[i].cord.x, "+");
    }
    if(room->window.x != 0 && room->window.x != 0){
        mvwprintw(game_win, room->window.y, room->window.x, "=");
    }
    if(room_number == 0 && current_level != 0){
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, "<");
    }
    else if(room_number == levels[current_level]->roomsCount - 1){
        mvwprintw(game_win, room->stair.cord.y, room->stair.cord.x, ">");
    }
}
void print_map(WINDOW *game_win){
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING;
    srand(time(NULL));
    // keypad(game_win, TRUE);
    draw_room(game_win, &rooms[0], 0);
    int k = 0;
    // for(int i = start_y; i < map_height; i++){
    //     for(int j = start_x; j < map_width; j++){
    //         mvwprintw(game_win, whole_map[k]->cord.y, whole_map[k]->cord.x, "%c", whole_map[k]->item);
    //         k++;
    //     }
    // }
    if(!strcmp(player->color, " Yellow ")){
        color_pair_num = 1;
        wattron(game_win, COLOR_PAIR(1));
    }
    else if(!strcmp(player->color, " Red ")){
        color_pair_num = 6;
        wattron(game_win, COLOR_PAIR(6));
    }
    else if(!strcmp(player->color, " Blue ")){
        color_pair_num = 9;
        wattron(game_win, COLOR_PAIR(9));
    }
    else if(!strcmp(player->color, " Green ")){
        color_pair_num = 7;
        wattron(game_win, COLOR_PAIR(7));
    }
    mvwprintw(game_win, player->cord.y, player->cord.x, "@");
    box(game_win, 0, 0);
    if(!strcmp(player->color, " Yellow ")){
        wattroff(game_win, COLOR_PAIR(1));
    }
    else if(!strcmp(player->color, " Red ")){
        wattroff(game_win, COLOR_PAIR(6));
    }
    else if(!strcmp(player->color, " Blue ")){
        wattroff(game_win, COLOR_PAIR(9));
    }
    else if(!strcmp(player->color, " Green ")){
        wattroff(game_win, COLOR_PAIR(7));
    }
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
        return;
    }
    else{
        for(int i = 0; i < 4; i++){
            if(whole_map2[current_level][next[i].y][next[i].x] == '#' || whole_map2[current_level][next[i].y][next[i].x] == '+'){
                if(whole_map2[current_level][next[i].y][next[i].x] == '#'){
                    wattron(game_win, COLOR_PAIR(3));
                }
                mvwprintw(game_win, next[i].y, next[i].x, "%c", whole_map2[current_level][next[i].y][next[i].x]);
                if(whole_map2[current_level][next[i].y][next[i].x] == '#'){
                    wattroff(game_win, COLOR_PAIR(3));
                }
                continue_corridor(game_win, next[i].y, next[i].x, a + 1);
            }
        }
    }
}

void player_move(WINDOW *game_win){
    int count_move = 1;
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING;
    int poisonous_food_check = 0;
    while(1){
        int c = wgetch(game_win);
        if(count_move % 4 == 2){
            mvwprintw(game_win, map_height - 7, 5, "                                                         ");
            mvwprintw(game_win, map_height - 5 + (player->foodCount) / 100, 5 + 2*(player->foodCount / 10) - 2*((player->foodCount)/100)*10, "  ");
        }
        if(whole_map2[current_level][player->cord.y][player->cord.x] == '#'){
            wattron(game_win, COLOR_PAIR(3));
        }
        if(c == 27){
            escape_check = 1;
            return;
        }
        else if(c == KEY_UP || c == 'w'){
            if(whole_map2[current_level][player->cord.y - 1][player->cord.x] != '|' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x] != '-' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x] != '_' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x] != 'O' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x] != '=' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x] != 0 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x] != ' '){
                mvwprintw(game_win, player->cord.y, player->cord.x, "%c", whole_map2[current_level][player->cord.y][player->cord.x]);
                player->cord.y -= 1;
            }
            else if(whole_map2[current_level][player->cord.y - 1][player->cord.x] == '='){
                int window_check = 0;
                for(int j = player->cord.y - 2; j < start_y; j++){
                    for(int k = player->cord.x - (j - player->cord.y + 2); k < player->cord.x + (j - player->cord.y + 2); k++){
                        if(k >= start_x && k <= end_x){
                            for(int i = 0; i < levels[current_level]->roomsCount; i++){
                                if(((k == levels[current_level]->rooms[i]->x
                                || k == levels[current_level]->rooms[i]->x + levels[current_level]->rooms[i]->width - 1)
                                &&(j >= levels[current_level]->rooms[i]->y 
                                && j <= levels[current_level]->rooms[i]->y + levels[current_level]->rooms[i]->height))
                                || ((j == levels[current_level]->rooms[i]->y
                                || j == levels[current_level]->rooms[i]->y + levels[current_level]->rooms[i]->height - 1)
                                && (k >= levels[current_level]->rooms[i]->x 
                                && k <= levels[current_level]->rooms[i]->x + levels[current_level]->rooms[i]->width))){
                                    draw_room(game_win, levels[current_level]->rooms[i], i);
                                    window_check = 1;
                                    break;
                                }
                            }
                        }
                        if(window_check){
                            break;
                        }
                    }
                    if(window_check){
                            break;
                        }
                }
            }
        }
        else if(c == KEY_DOWN || c == 's'){
            if(whole_map2[current_level][player->cord.y + 1][player->cord.x] != '|' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x] != '-' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x] != '_' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x] != 'O' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x] != '=' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x] != 0 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x] != ' '){
                mvwprintw(game_win, player->cord.y, player->cord.x, "%c", whole_map2[current_level][player->cord.y][player->cord.x]);
                player->cord.y += 1;
                count_move++;
            }
        }
        else if(c == KEY_RIGHT || c == 'd'){
            if(whole_map2[current_level][player->cord.y][player->cord.x] == '>'){
                if(unlocked_level < current_level){
                    unlocked_level = current_level;
                }
                current_level++;
                return;
            }
            if(whole_map2[current_level][player->cord.y][player->cord.x + 1] != '|' 
            && whole_map2[current_level][player->cord.y][player->cord.x + 1] != '-' 
            && whole_map2[current_level][player->cord.y][player->cord.x + 1] != '_' 
            && whole_map2[current_level][player->cord.y][player->cord.x + 1] != 'O' 
            && whole_map2[current_level][player->cord.y][player->cord.x + 1] != '=' 
            && whole_map2[current_level][player->cord.y][player->cord.x + 1] != 0
            && whole_map2[current_level][player->cord.y][player->cord.x + 1] != ' '){
                mvwprintw(game_win, player->cord.y, player->cord.x, "%c", whole_map2[current_level][player->cord.y][player->cord.x]);
                player->cord.x += 1;
                count_move++;
            }
        }
        else if(c == KEY_LEFT || c == 'a'){
            if(whole_map2[current_level][player->cord.y][player->cord.x] == '<'){
                current_level--;
                return;
            }
            if(whole_map2[current_level][player->cord.y][player->cord.x - 1] != '|' 
            && whole_map2[current_level][player->cord.y][player->cord.x - 1] != '-' 
            && whole_map2[current_level][player->cord.y][player->cord.x - 1] != '_' 
            && whole_map2[current_level][player->cord.y][player->cord.x - 1] != 'O' 
            && whole_map2[current_level][player->cord.y][player->cord.x - 1] != '=' 
            && whole_map2[current_level][player->cord.y][player->cord.x - 1] != 0
            && whole_map2[current_level][player->cord.y][player->cord.x - 1] != ' '){
                mvwprintw(game_win, player->cord.y, player->cord.x, "%c", whole_map2[current_level][player->cord.y][player->cord.x]);
                player->cord.x -= 1;
                count_move++;
            }
        }
        else if(c == KEY_HOME || c == 'q'){
            if(whole_map2[current_level][player->cord.y - 1][player->cord.x - 1] != '|' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x - 1] != '-' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x - 1] != '_' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x - 1] != 'O' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x - 1] != '=' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x - 1] != 0
            && whole_map2[current_level][player->cord.y - 1][player->cord.x - 1] != ' '){
                mvwprintw(game_win, player->cord.y, player->cord.x, "%c", whole_map2[current_level][player->cord.y][player->cord.x]);
                player->cord.x -= 1;
                player->cord.y -= 1;
                count_move++;
            }
        }
        else if(c == KEY_PPAGE || c == 'e'){
            if(whole_map2[current_level][player->cord.y - 1][player->cord.x + 1] != '|' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x + 1] != '-' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x + 1] != '_' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x + 1] != 'O' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x + 1] != '=' 
            && whole_map2[current_level][player->cord.y - 1][player->cord.x + 1] != 0
            && whole_map2[current_level][player->cord.y - 1][player->cord.x + 1] != ' '){
                mvwprintw(game_win, player->cord.y, player->cord.x, "%c", whole_map2[current_level][player->cord.y][player->cord.x]);
                player->cord.x += 1;
                player->cord.y -= 1;
                count_move++;
            }
        }
        else if(c == KEY_NPAGE || c == 'c'){
            if(whole_map2[current_level][player->cord.y + 1][player->cord.x + 1] != '|' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x + 1] != '-' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x + 1] != '_' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x + 1] != 'O' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x + 1] != '=' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x + 1] != 0 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x + 1] != ' '){
                mvwprintw(game_win, player->cord.y, player->cord.x, "%c", whole_map2[current_level][player->cord.y][player->cord.x]);
                player->cord.x += 1;
                player->cord.y += 1;
                count_move++;
            }
        }
        else if(c == KEY_END || c == 'z'){
            if(whole_map2[current_level][player->cord.y + 1][player->cord.x - 1] != '|' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x - 1] != '-' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x - 1] != '_' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x - 1] != 'O' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x - 1] != '=' 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x - 1] != 0 
            && whole_map2[current_level][player->cord.y + 1][player->cord.x - 1] != ' '){
                mvwprintw(game_win, player->cord.y, player->cord.x, "%c", whole_map2[current_level][player->cord.y][player->cord.x]);
                player->cord.x -= 1;
                player->cord.y += 1;
                count_move++;
            }
        }
        if(whole_map2[current_level][player->cord.y][player->cord.x] == '^'){
            for(int j = 0; j < levels[current_level]->rooms[player->room_num]->trapCount; j++){
                if(levels[current_level]->rooms[player->room_num]->traps[j].cord.x == player->cord.x 
                && levels[current_level]->rooms[player->room_num]->traps[j].cord.y == player->cord.y
                && levels[current_level]->rooms[player->room_num]->traps[j].isVisible != true){
                    levels[current_level]->rooms[player->room_num]->traps[j].isVisible = true;
                    player->health -= 5 * (difficulty_efficiency);
                }
            }
        }
        else if(whole_map2[current_level][player->cord.y][player->cord.x] == '%' && player->foodCount <= 150){
            for(int j = 0; j < levels[current_level]->rooms[player->room_num]->foodCount; j++){
                if(levels[current_level]->rooms[player->room_num]->foods[j].cord.x == player->cord.x 
                && levels[current_level]->rooms[player->room_num]->foods[j].cord.y == player->cord.y
                && levels[current_level]->rooms[player->room_num]->foods[j].isUsed != true){
                    levels[current_level]->rooms[player->room_num]->foods[j].isUsed = true;
                    if(difficulty_efficiency == 4 && rand() % 6 == 5){
                        poisonous_food_check = 1;
                        // for(int j = 0; j < player->foodCount; j++){
                        //     mvwprintw(game_win, map_height - 4 + j / 10, 5 + 2*j - 2*(j/10)*10, "🍖");
                        // }
                        mvwprintw(game_win, map_height - 7, 5, "Ops! Poisonous Meat O_o");
                        player->health -= 5;
                    }
                    else{
                        player->foodCount +=levels[current_level]->rooms[player->room_num]->foods[j].health;
                    }
                    count_move = 1;
                    whole_map2[current_level][player->cord.y][player->cord.x] = '.';
                }
            }
        }
        else if(whole_map2[current_level][player->cord.y][player->cord.x] == '$'){
            for(int j = 0; j < levels[current_level]->rooms[player->room_num]->goldCount; j++){
                if(levels[current_level]->rooms[player->room_num]->golds[j].cord.x == player->cord.x 
                && levels[current_level]->rooms[player->room_num]->golds[j].cord.y == player->cord.y
                && levels[current_level]->rooms[player->room_num]->golds[j].isUsed != true){
                    levels[current_level]->rooms[player->room_num]->golds[j].isUsed = true;
                    player->gold += levels[current_level]->rooms[player->room_num]->golds[j].count;
                    count_move = 1;
                    whole_map2[current_level][player->cord.y][player->cord.x] = '.';
                }
            }
        }
        wattroff(game_win, COLOR_PAIR(3));
        if(whole_map2[current_level][player->cord.y][player->cord.x] == '+'){
            for(int i = 0; i < levels[current_level]->roomsCount; i++){
                if(((player->cord.x == levels[current_level]->rooms[i]->x
                || player->cord.x == levels[current_level]->rooms[i]->x + levels[current_level]->rooms[i]->width - 1)
                &&(player->cord.y >= levels[current_level]->rooms[i]->y 
                && player->cord.y <= levels[current_level]->rooms[i]->y + levels[current_level]->rooms[i]->height))
                || ((player->cord.y == levels[current_level]->rooms[i]->y
                || player->cord.y == levels[current_level]->rooms[i]->y + levels[current_level]->rooms[i]->height - 1)
                && (player->cord.x >= levels[current_level]->rooms[i]->x 
                && player->cord.x <= levels[current_level]->rooms[i]->x + levels[current_level]->rooms[i]->width))){
                    player->room_num = i;
                    draw_room(game_win, levels[current_level]->rooms[i], i);
                    break;
                }
            }
        }
        if(whole_map2[current_level][player->cord.y][player->cord.x] == '+' || whole_map2[current_level][player->cord.y][player->cord.x] == '#'){
            continue_corridor(game_win, player->cord.y, player->cord.x, 0);
        }
        if(count_move % (45 / (difficulty_efficiency * 2 + 1)) == 0 && player->foodCount == 0){
            player->health -= 5;
            count_move = 1;
        }
        else if(count_move % (90 / (difficulty_efficiency * 2 + 1)) == 0 && player->foodCount > 0){
            player->foodCount -= 5;
            count_move = 1;
        }
        if(count_move % 4 == 2 && player->foodCount > 0 && player->health < 90){
            player->foodCount -= 10;
            player->health += 20;
        }
        wattron(game_win, COLOR_PAIR(color_pair_num));
        mvwprintw(game_win, player->cord.y, player->cord.x, "@");
        wattroff(game_win, COLOR_PAIR(color_pair_num));
        wattron(game_win, COLOR_PAIR(6));
        for(int j = 0; j < 20; j++){
            mvwprintw(game_win, map_height - 6, 5 + j, " ");
            mvwprintw(game_win, map_height - 5, 5 + j, " ");
            mvwprintw(game_win, map_height - 4, 5 + j, " ");
        }
        for(int j = 0; j < player->health / 10; j++){
            wchar_t heart = L'❤';
            mvwprintw(game_win, map_height - 6, 5 + 2*j, "❤");
        }
        if((player->health + 5) % 10 == 0){
            mvwprintw(game_win, map_height - 6, 4 + 2*player->health / 10, "💔");
        }
        wattroff(game_win, COLOR_PAIR(6));
        for(int j = 0; j < player->foodCount / 10; j++){
            mvwprintw(game_win, map_height - 5 + j / 10, 5 + 2*j - 2*(j/10)*10, "🍖");
        }
        if((player->foodCount + 5) % 10 == 0){
            mvwprintw(game_win,  map_height - 5 + player->foodCount / 100, 4 + 2*player->foodCount / 10 - 2*(player->foodCount / 100)*10, "🥓");
        }
        if(poisonous_food_check == 1){
            mvwprintw(game_win,  map_height - 5 + player->foodCount / 100, 5 + 2*player->foodCount / 10 - 2*(player->foodCount / 100)*10, "🥩");
            poisonous_food_check = 0;
        }
        wattron(game_win, COLOR_PAIR(12));
        mvwprintw(game_win, map_height - 3, 5, "💰%d$", player->gold);
        wattroff(game_win, COLOR_PAIR(12));
        wrefresh(game_win);
    }
}
void game_play(){
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
    strcpy(player->color, which_color);
    player->room_num = 0;
    if(current_level != 0){
        for(int i = 0; i < rooms[0].height; i++){
            int k = 0;
            for(int j = 0; j < rooms[0].width; j++){
                if(whole_map2[current_level][i][j] == '.'){
                    k = 1;
                    player->cord.x = rooms[0].x + i;
                    player->cord.y = rooms[0].y + j;
                    break;
                }
            }
            if(k){
                break;
            }
        }
    }
    // struct timeval tv;
    // gettimeofday(&tv, NULL);
    // milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    // // u = user;
    // // printf("%d , %d\n", maxX, maxY);
    // noecho();
    // damageTime -= (user->setting.level - 1) * 5;

    // // initilizing the game
    // game = (Game *)malloc(sizeof(Game));
    // game->levels = (Level **)malloc(4 * sizeof(Level *));
    // for (int i = 0; i < 4; i++)
    // {
    //     game->levels[i] = (Level *)malloc(sizeof(Level));
    //     createLevel(game->levels[i], i);
    // }
    // game->currentLevel = 0;

    // initialize player
    // player->cord.x = game->levels[game->currentLevel]->rooms[0]->cord.x + 2;
    // player->cord.y = game->levels[game->currentLevel]->rooms[0]->cord.y + 2;
    // player->state = 1;
    // player->room = game->levels[game->currentLevel]->rooms[0];
    if(new_game_check == 0){
        player->health = 100;
        player->foodCount = current_user.food;
        player->weaponCount = current_user.weaponsCount;
        player->spellCount = current_user.spellsCount;
        player->guns = (Gun *)malloc(30 * sizeof(Gun));
        player->spells = (Spell *)malloc(30 * sizeof(Spell));
        player->level = current_level;
        player->acientKey = current_user.ancientkey;
        player->brokenAcientKey = current_user.brokenancientkey;
        player->gold = current_user.golds;
        player->name = current_user.username;
    }
    else{
        player->health = 100;
        player->foodCount = 100;
        player->weaponCount = 0;
        player->spellCount = 0;
        player->guns = (Gun *)malloc(30 * sizeof(Gun));
        player->spells = (Spell *)malloc(30 * sizeof(Spell));
        player->level = current_level;
        player->acientKey = 0;
        player->brokenAcientKey = 0;
        player->gold = current_user.golds;
        player->name = current_user.username;
    }
    new_game_check = 0;
    // game->player = player;.
    keypad(game_win, TRUE);
    if(unlocked_level >= current_level){
        for(int i = start_y; i < map_height; i++){
            for(int j = start_x; j < map_width; j++){
                mvwprintw(game_win, i, j, "%c", whole_map2[current_level][i][j]);
            }
        }
    }
    print_map(game_win);
    player_move(game_win);
    clear();
    if(escape_check){
        delwin(game_win);
        return;
    }
    delwin(game_win);
}