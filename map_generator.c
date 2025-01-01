#include "map_generator.h"

void draw_room(Room room) {
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            if (y == room.y) {
                mvaddch(y, x, '_');
            } else if (y == room.y + room.height - 1) {
                mvaddch(y, x, '_');
                if(x == room.x || x == room.x + room.width - 1){
                    mvaddch(y, x, '|');
                }
            } else if (x == room.x || x == room.x + room.width - 1) {
                mvaddch(y, x, '|');
            } else {
                mvaddch(y, x, '.');
            }
        }
    }
}

void draw_windows(Room *room) {
    int num_windows = 1 + rand() % 1;
    int side = 4, last_side = 4;
    for (int i = 0; i < num_windows; i++) {
        while(last_side == side){
            side = rand() % 4;
        }
        last_side = side;
        int position;
        switch (side) {
            case 0: // مرز بالا
                position = room->x + 1 + rand() % (room->width - 2);
                mvaddch(room->y, position, '=');
                room->door_x = position;
                room->door_y = room->y;
                break;
            case 1: // مرز پایین
                position = room->x + 1 + rand() % (room->width - 2);
                mvaddch(room->y + room->height - 1, position, '=');
                room->door_x = position;
                room->door_y = room->y + room->height - 1;
                break;
            case 2: // مرز چپ
                position = room->y + 1 + rand() % (room->height - 2);
                mvaddch(position, room->x, '=');
                room->door_x = room->x;
                room->door_y = position;
                break;
            case 3: // مرز راست
                position = room->y + 1 + rand() % (room->height - 2);
                mvaddch(position, room->x + room->width - 1, '=');
                room->door_x = room->x + room->width - 1;
                room->door_y = position;
                break;
        }
    }
}
void draw_doors(Room *room) {
    int num_doors = 1 + rand() % 2;
    int side = 4, last_side = 4;
    for (int i = 0; i < num_doors; i++) {
        while(last_side == side){
            side = rand() % 4;
        }
        last_side = side;
        int position;
        switch (side) {
            case 0: // مرز بالا
                position = room->x + 1 + rand() % (room->width - 2);
                mvaddch(room->y, position, '+');
                room->door_x = position;
                room->door_y = room->y;
                break;
            case 1: // مرز پایین
                position = room->x + 1 + rand() % (room->width - 2);
                mvaddch(room->y + room->height - 1, position, '+');
                room->door_x = position;
                room->door_y = room->y + room->height - 1;
                break;
            case 2: // مرز چپ
                position = room->y + 1 + rand() % (room->height - 2);
                mvaddch(position, room->x, '+');
                room->door_x = room->x;
                room->door_y = position;
                break;
            case 3: // مرز راست
                position = room->y + 1 + rand() % (room->height - 2);
                mvaddch(position, room->x + room->width - 1, '+');
                room->door_x = room->x + room->width - 1;
                room->door_y = position;
                break;
        }
    }
}

void draw_objects(Room room) {
    int num_objects = 1 + rand() % 3; // 1 تا 3 شیء رندم (کاراکتر 'O') داخل اتاق
    for (int i = 0; i < num_objects; i++) {
        int x = room.x + 1 + rand() % (room.width - 2); // انتخاب تصادفی x داخل اتاق
        int y = room.y + 1 + rand() % (room.height - 2); // انتخاب تصادفی y داخل اتاق
        mvaddch(y, x, 'O'); // قرار دادن 'O' در موقعیت تصادفی
    }
}

int intersects(Room *rooms, int num_rooms, Room room) {
    for (int i = 0; i < num_rooms; i++) {
        if (room.x < rooms[i].x + rooms[i].width + ROOM_PADDING &&
            room.x + room.width + ROOM_PADDING > rooms[i].x &&
            room.y < rooms[i].y + rooms[i].height + ROOM_PADDING &&
            room.y + room.height + ROOM_PADDING > rooms[i].y) {
            return 1; // تداخل دارد
        }
    }
    return 0; // تداخل ندارد
}

int is_valid_move(int x, int y, Room *rooms, int num_rooms) {
    for (int i = 0; i < num_rooms; i++) {
        if (x >= rooms[i].x && x < rooms[i].x + rooms[i].width &&
            y >= rooms[i].y && y < rooms[i].y + rooms[i].height) {
            return 0; // نقطه در داخل اتاق است
        }
    }
    return 1; // نقطه آزاد است
}

void connect_rooms(Room room1, Room room2, Room *rooms, int num_rooms) {
    int x1 = room1.door_x;
    int y1 = room1.door_y;
    int x2 = room2.door_x;
    int y2 = room2.door_y;

    // حرکت افقی از x1 به x2
    if (x1 != x2) {
        if (x1 < x2) {
            for (int x = x1; x <= x2; x++) {
                if (is_valid_move(x, y1, rooms, num_rooms)) {
                    mvaddch(y1, x, '#');
                }
            }
        } else {
            for (int x = x1; x >= x2; x--) {
                if (is_valid_move(x, y1, rooms, num_rooms)) {
                    mvaddch(y1, x, '#');
                }
            }
        }
    }

    // حرکت عمودی از y1 به y2
    if (y1 != y2) {
        if (y1 < y2) {
            for (int y = y1; y <= y2; y++) {
                if (is_valid_move(x2, y, rooms, num_rooms)) {
                    mvaddch(y, x2, '#');
                }
            }
        } else {
            for (int y = y1; y >= y2; y--) {
                if (is_valid_move(x2, y, rooms, num_rooms)) {
                    mvaddch(y, x2, '#');
                }
            }
        }
    }
}

void generate_map() {
    clear();
    Room rooms[10];
    int num_rooms = 0;

    int map_width, map_height;
    getmaxyx(stdscr, map_height, map_width); // دریافت اندازه‌ی واقعی ترمینال

    srand(time(NULL)); // تنظیم seed برای تولید تصادفی

    // تعیین تعداد تصادفی اتاق‌ها بین 6 تا 10
    int total_rooms = 6 + rand() % 3;

    // تنظیم فضای شروع اتاق‌ها به طوری که حداقل 2 خانه از مرزهای صفحه فاصله داشته باشند
    int start_x = BORDER_PADDING;
    int start_y = BORDER_PADDING;
    int end_x = map_width - BORDER_PADDING;
    int end_y = map_height - BORDER_PADDING;

    while (num_rooms < total_rooms) {
        Room room;
        room.width = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        room.height = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);

        room.x = start_x + rand() % (end_x - room.width - start_x); // محدوده برای شروع
        room.y = start_y + rand() % (end_y - room.height - start_y); // محدوده برای شروع

        // بررسی تداخل و افزودن اتاق
        if (!intersects(rooms, num_rooms, room)) {
            draw_room(room);
            draw_doors(&room); // رسم درها
            draw_windows(&room); // رسم درها
            draw_objects(room); // قرار دادن شیء ('O') در داخل اتاق
            if (num_rooms > 0) {
                connect_rooms(rooms[num_rooms - 1], room, rooms, num_rooms); // اتصال اتاق‌ها با راهرو
            }
            rooms[num_rooms++] = room;
        }
    }

    // رسم بردر در اطراف کل صفحه
    box(stdscr, 0, 0);

    mvprintw(0, 0, "Generated Map:");
    refresh();
}
