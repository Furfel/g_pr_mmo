#ifndef GAME_H
#define GAME_H

#include <pthread.h>
#include "utils.h"

#define MUTEX_CHUNK_W 16
#define MUTEX_CHUNK_H 16
#define MAX_TILE_DEPTH 24
#define NOITEM 0xFF
#define NO_TILE_SPACE -1
#define ITEM_PUSHED 1
#define MAX_BULLETS 64
#define BULLET_EMPTY 0
#define BULLET_FIRE 1
#define BULLET_EARTH 2
#define BULLET_KNIFE 3
#define BULLET_FIRE_DISTANCE 14.0f
#define BULLET_EARTH_DISTANCE 8.0f
#define BULLET_KNIFE_DISTANCE 14.0f
#define BULLET_FIRE_SPEED 0.1f
#define BULLET_EARTH_SPEED 0.1f
#define BULLET_KNIFE_SPEED 0.1f

typedef struct {
	unsigned char type;
	float start_x;
	float start_y;
	float angle;
	float x;
	float y;
	float r;
} Bullet;

typedef struct {
	unsigned char items[MAX_TILE_DEPTH];
	unsigned char count;
} Tile;

pthread_mutex_t** chunk_locks;
Tile** map;
unsigned int MapWidth;
unsigned int MapHeight;

Bullet bullets[MAX_BULLETS];
unsigned int bullet_id;
pthread_mutex_t bullet_lock;

Tile** CreateMap(unsigned int map_width, unsigned int map_height);
void DestroyMap(Tile** tiles, pthread_mutex_t** chunklocks);
void ReadMap(Tile*** dst, const char* name);
short PushTile(Tile* tile, char item);
unsigned char PopTile(Tile* tile);
unsigned char TopTile(Tile* tile);
signed char IsBlockingXY(unsigned short, unsigned short);
void InitBullets();
void CreateBullet(float, float, float);
void StartGame(Thread*);
void DumpMap(int,int,int,int);


#endif
