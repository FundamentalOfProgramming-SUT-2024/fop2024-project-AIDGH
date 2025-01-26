#ifndef GAME_H
#define GAME_H

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    Point cord;
    char item;
} Map;

typedef struct
{
    Point points[100]; // (x,y) of the way
    int count;
    int index;
    int visiblePoint;
} Corridor;

typedef struct
{
    Point cord;
    char type; // 'n' -> normal 's' -> secret 'l' -> locked  'h' -> hard locked
    int password;
} Door;

typedef struct
{
    int health;
    bool isUsed;
    Point cord;
} Food;

typedef struct
{
    char type; // g -> gorz; k -> khanjar; a -> asa; t -> tir; s-> shamshir
    bool isUsed;
    Point cord;
} Gun;

typedef struct
{
    char type;
    bool isUsed;
    Point cord;
} Spell;

typedef struct
{
    int count;
    char type;
    bool isUsed;
    Point cord;
} Gold;

typedef struct
{
    int damage;
    Point cord;
    bool isVisible;
} Trap;

typedef struct
{
    int from; // source level
    int to;   // dest level
    Point cord;
} Stair;

typedef struct
{
    int x, y;
    int height;
    int width;
    Door *doors;
    int doorCount;
    Point window;
    Point *pillars;
    bool isVisible;
    int index;
    Food *foods;
    Gold *golds;
    Gun *guns;
    Spell *spells;
    Trap *traps;
    Stair stair;
    int pillarcount;
    int keyCount;
    int foodCount;
    int goldCount;
    int gunCount;
    int spellCount;
    int trapCount;
    char type;
    int stairCount;
} Room;

typedef struct
{
    Point cord;
    int health;
    int state; // 0 -> in Corridor ; 1 -> in room
    Room *room;
    Corridor *Corridor;
    Gun* guns;
    Spell* spells;
    int foodCount;
    int weaponCount;
    int spellCount;
    int room_num;
    int level;
    char *name;
    char color[10];
    int acientKey;
    int gold;
    int brokenAcientKey;
} Player;

typedef struct
{
    Room **rooms;
    Corridor **Corridors;
    Point key;
    int roomsCount;
    int level;
} Level;

typedef struct
{
    Level **levels;
    Player *player;

    int currentLevel;
} Game;

void game_play();
void print_map(WINDOW *game_win);
void draw_room(WINDOW *game_win, Room *room, int room_number);

#endif