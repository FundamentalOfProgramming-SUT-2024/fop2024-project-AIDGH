#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define ROOM_MIN_SIZE 10
#define ROOM_MAX_SIZE 20
#define ROOM_PADDING 2  // فاصله بین مستطیل‌ها
#define BORDER_PADDING 2 // فاصله بین اتاق‌ها و دیوارهای صفحه

typedef struct {
    int x, y, width, height;
    int door_x, door_y; // موقعیت دروازه اتاق
} Room;

void draw_room(Room room);
void draw_doors(Room *room);
void draw_objects(Room room);
int intersects(Room *rooms, int num_rooms, Room room);
int is_valid_move(int x, int y, Room *rooms, int num_rooms);
void connect_rooms(Room room1, Room room2, Room *rooms, int num_rooms);
void generate_map();

#endif // MAP_GENERATOR_H
