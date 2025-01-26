#include "map_generator.h"
#include "game.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <string.h>

extern char which_color[50];
extern Room rooms[10];
extern Player *player;
extern Map *whole_map[20000];
extern char whole_map2[6][200][200];
extern Corridor **corridors;
int k = 0, k1 = 0;
int total_rooms;
extern Level *levels[6];
extern int current_level;
extern int unlocked_level;

void create_room(Room *room) {
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            if (y == room->y) {
                mvaddch(y, x, '_');
            } else if (y == room->y + room->height - 1) {
                mvaddch(y, x, '-');
                // if(x == room->x || x == room->x + room->width - 1){
                //     mvaddch(y, x, '|');
                // }
            } else if (x == room->x || x == room->x + room->width - 1) {
                mvaddch(y, x, '|');
            } else {
                mvaddch(y, x, '.');
            }
        }
    }
}

void correct_room(Room *room) {
    int count = 0;
    int check1 = 0, check2 = 0, check3 = 0, check4 = 0;
    room->doors = malloc(10 * sizeof(Door));
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            if(y == room->y && (mvwinch(stdscr, y - 1, x) == '#' || mvwinch(stdscr, y - 2, x) == '#') && check1 != 2){
                mvaddch(y - 1, x, '#');
                room->doors[count].type = 'n';
                room->doors[count].cord.x = x;
                room->doors[count].cord.y = y;
                count++;
                mvaddch(y, x, '+');
                check1 += 1;
            }
            else if(y == room->y + room->height - 1 && (mvwinch(stdscr, y + 1, x) == '#' || mvwinch(stdscr, y + 2, x) == '#') && check2 != 2){
                mvaddch(y + 1, x, '#');
                room->doors[count].type = 'n';
                room->doors[count].cord.x = x;
                room->doors[count].cord.y = y;
                count++;
                mvaddch(y, x, '+');
                check2 += 1;
            }
            else if(x == room->x && (mvwinch(stdscr, y, x - 1) == '#' || mvwinch(stdscr, y, x - 2) == '#') && check3 != 2){
                mvaddch(y, x - 1, '#');
                room->doors[count].type = 'n';
                room->doors[count].cord.x = x;
                room->doors[count].cord.y = y;
                count++;
                mvaddch(y, x, '+');
                check3 += 1;
            }
            else if(x == room->x + room->width - 1 && (mvwinch(stdscr, y, x + 1) == '#' || mvwinch(stdscr, y, x + 2) == '#') && check4 != 2){
                mvaddch(y, x + 1, '#');
                room->doors[count].type = 'n';
                room->doors[count].cord.x = x;
                room->doors[count].cord.y = y;
                count++;
                mvaddch(y, x, '+');
                check4 += 1;
            }
            else if (y == room->y && mvwinch(stdscr, y, x) == '#') {
                room->doors[count].type = 'n';
                room->doors[count].cord.x = x;
                room->doors[count].cord.y = y;
                count++;
                mvaddch(y, x, '+');
            } else if (y == room->y + room->height - 1 && mvwinch(stdscr, y, x) == '#') {
                room->doors[count].type = 'n';
                room->doors[count].cord.x = x;
                room->doors[count].cord.y = y;
                count++;
                mvaddch(y, x, '+');
            } else if ((x == room->x || x == room->x + room->width - 1) && mvwinch(stdscr, y, x) == '#') {
                room->doors[count].type = 'n';
                room->doors[count].cord.x = x;
                room->doors[count].cord.y = y;
                count++;
                mvaddch(y, x, '+');
            } else {
                if(mvwinch(stdscr, y, x) == '#')
                mvaddch(y, x, '.');
            }
        }
    }
    if(mvwinch(stdscr, room->y, room->x) == '+' && (mvwinch(stdscr, room->y, room->x + 1) != '+' && mvwinch(stdscr, room->y + 1, room->x) != '+')){
        mvaddch(room->y + 1, room->x, '+');
        room->doors[count].type = 'n';
        room->doors[count].cord.x = room->x;
        room->doors[count].cord.y = room->y;
        count++;
    }
    if(mvwinch(stdscr, room->y + room->height - 1, room->x) == '+' && 
    (mvwinch(stdscr, room->y + room->height - 1, room->x + 1) != '+' && mvwinch(stdscr, room->y + room->height - 2, room->x) != '+')){
        mvaddch(room->y  + room->height - 1, room->x + 1, '+');
        room->doors[count].type = 'n';
        room->doors[count].cord.x = room->x + 1;
        room->doors[count].cord.y = room->y  + room->height - 1;
        count++;
    }
    if(mvwinch(stdscr, room->y + room->height - 1, room->x + room->width - 1) == '+' && 
    (mvwinch(stdscr, room->y + room->height - 1, room->x + room->width - 2) != '+' && 
    mvwinch(stdscr, room->y + room->height - 2, room->x + room->width - 1) != '+')){
        mvaddch(room->y  + room->height - 1, room->x  + room->width - 2, '+');
        room->doors[count].type = 'n';
        room->doors[count].cord.x = room->x + room->width - 2;
        room->doors[count].cord.y = room->y + room->height - 1;
        count++;
    }
    if(mvwinch(stdscr, room->y, room->x + room->width - 1) == '+' && (mvwinch(stdscr, room->y, room->x + room->width - 2) != '+'
     && mvwinch(stdscr, room->y + 1, room->x + room->width - 1) != '+')){
        mvaddch(room->y + 1, room->x + room->width - 1, '+');
        room->doors[count].type = 'n';
        room->doors[count].cord.x = room->x  + room->width - 1;
        room->doors[count].cord.y = room->y + 1;
        count++;
    }
    for (int y = room->y; y < room->y + room->height; y++) {
        for (int x = room->x; x < room->x + room->width; x++) {
            if(mvwinch(stdscr, y, x) == 'O' && (mvwinch(stdscr, y - 1, x) == '+' || mvwinch(stdscr, y + 1, x) == '+'
             || mvwinch(stdscr, y, x - 1) == '+' || mvwinch(stdscr, y, x + 1) == '+')){
                mvaddch(y, x, '.');
             }
        }
    }
    room->doorCount = count;
}

void draw_windows(Room *room) {
    int num_windows = rand() % 2;
    int side = 4, last_side = 4;
    room->window.x = 0;
    room->window.y = 0;
    for (int i = 0; i < num_windows; i++) {
        while(last_side == side){
            side = rand() % 4;
        }
        last_side = side;
        int position;
        switch (side) {
            case 0:
                position = room->x + 1 + rand() % (room->width - 2);
                if(mvwinch(stdscr, room->y, position) != '|' && 
                mvwinch(stdscr, room->y, position) != '-' && 
                mvwinch(stdscr, room->y, position) != '_'){
                    draw_windows(room);
                }
                mvaddch(room->y, position, '=');
                room->window.x = position;
                room->window.y = room->y;
                break;
            case 1:
                position = room->x + 1 + rand() % (room->width - 2);
                if(mvwinch(stdscr, room->y + room->height - 1, position) != '|' && 
                mvwinch(stdscr, room->y + room->height - 1, position) != '-' && 
                mvwinch(stdscr, room->y + room->height - 1, position) != '_'){
                    draw_windows(room);
                }
                mvaddch(room->y + room->height - 1, position, '=');
                room->window.x = position;
                room->window.y = room->y + room->height - 1;
                break;
            case 2:
                position = room->y + 1 + rand() % (room->height - 2);
                if(mvwinch(stdscr, position, room->x) != '|' && 
                mvwinch(stdscr, position, room->x) != '-' && 
                mvwinch(stdscr, position, room->x) != '_'){
                    draw_windows(room);
                }
                mvaddch(position, room->x, '=');
                room->window.x = room->x;
                room->window.y = position;
                break;
            case 3:
                position = room->y + 1 + rand() % (room->height - 2);
                if(mvwinch(stdscr, position, room->x + room->width - 1) != '|' && 
                mvwinch(stdscr, position, room->x + room->width - 1) != '-' && 
                mvwinch(stdscr, position, room->x + room->width - 1) != '_'){
                    draw_windows(room);
                }
                mvaddch(position, room->x + room->width - 1, '=');
                room->window.x = room->x + room->width - 1;
                room->window.y = position;
                break;
        }
    }
}

void create_pillars(Room *room) {
    int num_objects = 1 + rand() % 3;
    room->pillars = malloc(num_objects * sizeof(Point));
    room->pillarcount = num_objects;
    for (int i = 0; i < num_objects; i++) {
        int x = room->x + 2 + rand() % (room->width - 4);
        int y = room->y + 2 + rand() % (room->height - 4);
        mvaddch(y, x, 'O');
        room->pillars[i].x = x;
        room->pillars[i].y = y;
    }
}
void create_food(Room *room) {
    int num_objects = 1 + rand() % 3;
    room->foods = malloc(num_objects * sizeof(Food));
    room->foodCount = num_objects;
    for (int i = 0; i < num_objects; i++) {
        int x = room->x + 1 + rand() % (room->width - 2);
        int y = room->y + 1 + rand() % (room->height - 2);
        if(mvwinch(stdscr, y, x) == '.'){
            mvaddch(y, x, '%');
            room->foods[i].cord.x = x;
            room->foods[i].cord.y = y;
            room->foods[i].health = (rand() % 4 + 1) * 5;
            room->foods[i].isUsed = false;
        }
        else{
            i--;
        }
    }
}
void create_gold(Room *room) {
    int num_objects = 1 + rand() % 3;
    room->golds = malloc(num_objects * sizeof(Gold));
    room->goldCount = num_objects;
    for (int i = 0; i < num_objects; i++) {
        int x = room->x + 2 + rand() % (room->width - 4);
        int y = room->y + 2 + rand() % (room->height - 4);
        if(mvwinch(stdscr, y, x) == '.'){
            mvaddch(y, x, '$');
            room->golds[i].cord.x = x;
            room->golds[i].cord.y = y;
            room->golds[i].count = (rand() % 4 + 1) * 5;
            room->golds[i].isUsed = false;
        }
        else{
            i--;
        }
    }
}
void draw_traps(Room *room) {
    int num_objects = 1 + rand() % 2;
    room->traps = malloc(num_objects * sizeof(Trap));
    room->trapCount = num_objects;
    for (int i = 0; i < num_objects; i++) {
        int x = room->x + 1 + rand() % (room->width - 2);
        int y = room->y + 1 + rand() % (room->height - 2);
        if(mvwinch(stdscr, y, x) == '.'){
            mvaddch(y, x, '^');
            room->traps[i].cord.x = x;
            room->traps[i].cord.y = y;
            room->traps[i].isVisible = false;
        }
        else{
            i--;
        }
    }
}
void draw_stair_1(Room *room) {
    int x = room->x + 2 + rand() % (room->width - 3);
    int y = room->y + 2 + rand() % (room->height - 3);
    if(mvwinch(stdscr, y, x) == '.'){
        mvaddch(y, x, '>');
        room->stair.from = current_level;
        room->stair.to = current_level + 1;
        room->stair.cord.x = x;
        room->stair.cord.y = y;
    }
    else{
        draw_stair_1(room);
    }
}
void draw_stair_2(Room *room) {
    int x = room->x + 2 + rand() % (room->width - 3);
    int y = room->y + 2 + rand() % (room->height - 3);
    if(mvwinch(stdscr, y, x) == '.'){
        mvaddch(y, x, '<');
        room->stair.from = current_level;
        room->stair.to = current_level - 1;
        room->stair.cord.x = x;
        room->stair.cord.y = y;
    }
    else{
        draw_stair_2(room);
    }
}
void draw_player(Room *room) {
    int x = room->x + 2 + rand() % (room->width - 3);
    int y = room->y + 2 + rand() % (room->height - 3);
    if(mvwinch(stdscr, y, x) == '.'){
        if(!strcmp(which_color, " Yellow ")){
            attron(COLOR_PAIR(1));
        }
        else if(!strcmp(which_color, " Red ")){
            attron(COLOR_PAIR(6));
        }
        else if(!strcmp(which_color, " Blue ")){
            attron(COLOR_PAIR(9));
        }
        else if(!strcmp(which_color, " Green ")){
            attron(COLOR_PAIR(7));
        }
        mvaddch(y, x, '@');
        player->cord.x = x;
        player->cord.y = y;
        if(!strcmp(which_color, " Yellow ")){
            attroff(COLOR_PAIR(1));
        }
        else if(!strcmp(which_color, " Red ")){
            attroff(COLOR_PAIR(6));
        }
        else if(!strcmp(which_color, " Blue ")){
            attroff(COLOR_PAIR(9));
        }
        else if(!strcmp(which_color, " Green ")){
            attroff(COLOR_PAIR(7));
        }
    }
    else{
        draw_player(room);
    }
}

int intersects(Room *rooms, int num_rooms, Room room, int total_rooms) {
    for (int i = 0; i < num_rooms; i++) {
        if (room.x < rooms[i].x + rooms[i].width + ROOM_PADDING + 4 &&
            room.x + room.width + ROOM_PADDING > rooms[i].x - 4 &&
            room.y < rooms[i].y + rooms[i].height + ROOM_PADDING + 4 &&
            room.y + room.height + ROOM_PADDING > rooms[i].y - 4) {
            return 1;
        }
    }
    if (room.x < rooms[total_rooms - 1].x + rooms[total_rooms - 1].width + ROOM_PADDING + 4 &&
        room.x + room.width + ROOM_PADDING > rooms[total_rooms - 1].x - 4 &&
        room.y < rooms[total_rooms - 1].y + rooms[total_rooms - 1].height + ROOM_PADDING + 4 &&
        room.y + room.height + ROOM_PADDING > rooms[total_rooms - 1].y - 4) {
        return 1;
    }
    return 0;
}

int is_valid_move(int x, int y, Room *rooms, int num_rooms) {
    for (int i = 0; i < num_rooms; i++) {
        if (x >= rooms[i].x && x < rooms[i].x + rooms[i].width &&
            y >= rooms[i].y && y < rooms[i].y + rooms[i].height) {
            return 0;
        }
    }
    return 1;
}
int check_rooms_validate(int room_checker[1000][2], int t, int num_rooms){
    int a = 0, b = 0, c = 0;
    for(int i = 0; i < t; i++){
        if(room_checker[i][0] == 0 && b == 0){
            a++;
            b = 1;
        }
        if(room_checker[i][0] == num_rooms - 1 && c == 0){
            a++;
            c = 1;
        }
    }
    if(a == 2){
        return 0;
    }
    return 1;
}
void connect_random_rooms(Room *rooms, int num_rooms, int room_checker[1000][2], int *t) {
    if (num_rooms < 2) return;

    int room1_index = rand() % num_rooms;
    int room2_index;
    do {
        room2_index = rand() % num_rooms;
    } while (room2_index == room1_index);

    for (int i = 0; i < *t; i++){
        if((room_checker[i][0] == room1_index && room_checker[i][1] == room2_index)){
            return ;
        }
    }

    room_checker[*t][0] = room1_index;
    room_checker[*t][1] = room2_index;
    (*t)++;
    Room room1 = rooms[room1_index];
    Room room2 = rooms[room2_index];

    int side1 = rand() % 4;
    int side2 = rand() % 4;

    int x1, y1, x2, y2;

    switch (side1) {
        case 0:
            x1 = room1.x + 1 + rand() % (room1.width - 3);
            y1 = room1.y;
            break;
        case 1:
            x1 = room1.x + 1 + rand() % (room1.width - 3);
            y1 = room1.y + room1.height - 1;
            break;
        case 2:
            x1 = room1.x;
            y1 = room1.y + 1 + rand() % (room1.height - 3);
            break;
        case 3:
            x1 = room1.x + room1.width - 1;
            y1 = room1.y + 1 + rand() % (room1.height - 3);
            break;
    }

    switch (side2) {
        case 0:
            x2 = room2.x + 1 + rand() % (room2.width - 3);
            y2 = room2.y;
            break;
        case 1:
            x2 = room2.x + 1 + rand() % (room2.width - 3);
            y2 = room2.y + room2.height - 1;
            break;
        case 2:
            x2 = room2.x;
            y2 = room2.y + 1 + rand() % (room2.height - 3);
            break;
        case 3:
            x2 = room2.x + room2.width - 1;
            y2 = room2.y + 1 + rand() % (room2.height - 3);
            break;
    }

    int dx = (x2 > x1) ? 1 : (x2 < x1) ? -1 : 0;
    int dy = (y2 > y1) ? 1 : (y2 < y1) ? -1 : 0;

    int x = x1 + dx, y = y1 + dy;
    k1 = 0;
    while (x != x2 || y != y2) {
        for(int i = 0; i < rand () % 15 && x != x2; i++){
            if(!is_valid_move(x, y, rooms, num_rooms)){
                if(x != x1 + dx){
                    //corridors[k]->points[k1].x = x;
                    //corridors[k]->points[k1].y = y;
                    mvaddch(y, x, '#');
                    k1++;
                    //corridors[k]->points[k1].x = x + dx;
                    //corridors[k]->points[k1].y = y;
                    k1++;
                    mvaddch(y, x + dx, '#');
                    // mvaddch(x1, y1, '#');
                    //corridors[k]->index = k;
                    //corridors[k]->count = k1;
                }
                else{
                    k--;
                    if(room_checker[*t - 1][0] == 0){
                        room_checker[*t - 1][0] = 100;
                        room_checker[*t - 1][1] = 100;
                    }
                }
                return ;
            }
            else{
                //corridors[k]->points[k1].x = x;
                //corridors[k]->points[k1].y = y;
                k1++;
                mvaddch(y, x, '#');
                x += dx;
            }
        }
        for(int i = 0; i < rand () % 15 && y != y2; i++){
            if(!is_valid_move(x, y, rooms, num_rooms)){
                if(y != y1 + dx){
                    //corridors[k]->points[k1].x = x;
                    //corridors[k]->points[k1].y = y;
                    k1++;
                    mvaddch(y, x, '#');
                    //corridors[k]->points[k1].x = x;
                    //corridors[k]->points[k1].y = y + dy;
                    k1++;
                    mvaddch(y + dy, x, '#');
                    //corridors[k]->index = k;
                    //corridors[k]->count = k1;
                    k++;
                }
                else{
                    k--;
                    if(room_checker[*t - 1][0] == num_rooms - 1){
                        room_checker[*t - 1][0] = 100;
                        room_checker[*t - 1][1] = 100;
                    }
                }
                return ;
            }
            else{
                //corridors[k]->points[k1].x = x;
                //corridors[k]->points[k1].y = y;
                k1++;
                mvaddch(y, x, '#');
                y += dy;
            }
        }
    }
    //corridors[k]->index = k;
    //corridors[k]->count = k1;
    mvaddch(x1, y1, '#');
    // mvaddch(x, y, '#');
    return;
}

void generate_map() {
    clear();
    // Room rooms[10];
    int num_rooms = 0;
    int room_checker[1000][2], t = 0;
    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width);
    srand(time(NULL));
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    corridors = malloc(50 * sizeof(Corridor *));
    for(int i = 0; i < 50; i++){
        corridors[i] = malloc(1 * sizeof(Corridor));
    }
    total_rooms = 6 + rand() % 3;
    levels[current_level] = malloc(sizeof(Level));
    levels[current_level]->rooms = malloc(total_rooms * sizeof(Room*));
    levels[current_level]->roomsCount = total_rooms;
    for(int i = 0; i < total_rooms; i++){
        levels[current_level]->rooms[i] = malloc(sizeof(Room));
    }
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    // Giving floor size
    int end_x = map_width * 3 / 4 - BORDER_PADDING;
    int end_y = map_height * 3 / 4 - BORDER_PADDING;
    player = (Player *)malloc(sizeof(Player));
    Room room;
    room.width = ROOM_MIN_SIZE + 2 + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
    room.height = ROOM_MIN_SIZE + 2 + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
    room.x = start_x + end_x - 20 - rand() % (25 - room.width - start_x);
    room.y = start_y + end_y - 10 - rand() % (25 - room.height - start_y);
    create_room(&room);
    rooms[total_rooms - 1] = room;
    
    while (num_rooms < total_rooms - 1) {
        Room room;
        room.width = ROOM_MIN_SIZE + 2 + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        room.height = ROOM_MIN_SIZE + 2 + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);

        room.x = start_x + rand() % (end_x - room.width - start_x);
        room.y = start_y + rand() % (end_y - room.height - start_y);

        if(num_rooms == 0){
            room.x = start_x + rand() % (25 - room.width - start_x);
            room.y = start_y + rand() % (25 - room.height - start_y);
            create_room(&room);
            rooms[num_rooms++] = room;
        }

        if (!intersects(rooms, num_rooms, room, total_rooms)) {
            create_room(&room);
            rooms[num_rooms++] = room;
        }
    }
    num_rooms++;

    int k = 0;
    while(k < total_rooms * total_rooms || check_rooms_validate(room_checker, t, num_rooms)){
        connect_random_rooms(rooms, num_rooms, room_checker, &t);
        k++;
    }

    for (int i = 0; i < num_rooms; i++) {
        create_pillars(&rooms[i]);
        create_food(&rooms[i]);
        create_gold(&rooms[i]);
        draw_traps(&rooms[i]);
        draw_windows(rooms + i);
    }
    draw_stair_1(&rooms[num_rooms - 1]);
    if(current_level != 0){
        draw_stair_2(&rooms[0]);
    }
    draw_player(&rooms[0]);
    for (int i = 0; i < num_rooms; i++) {
        correct_room(&rooms[i]);
    }
    for(int i = 0; i < total_rooms; i++){
        levels[current_level]->rooms[i] = &rooms[i];
    }
    k = 0;
    for(int i = start_y; i < map_height; i++){
        for(int j = start_x; j < map_width; j++){
            whole_map[k] = malloc(1 * sizeof(Map));
            whole_map[k]->cord.x = j;
            whole_map[k]->cord.y = i;
            if(i == player->cord.y && j == player->cord.x){
                whole_map[k]->item = '.';
                whole_map2[current_level][i][j] = '.';
            }
            else{
                whole_map[k]->item = mvwinch(stdscr, i, j);
                whole_map2[current_level][i][j] = mvwinch(stdscr, i, j);
            }
            k++;
        }
    }
    // box(stdscr, 0, 0);
    // for(int i = start_y; i < end_y; i++){
    //     for(int j = start_x; j < end_x; j++){
    //         if(mvwinch(stdscr, i, j) == '#')
    //     }
    // }
    // mvprintw(0, 0, "Generated Map:");
    refresh();
    // getch();
    // clear();
}

