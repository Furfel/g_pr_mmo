#ifndef GAME_H
#define GAME_H

#include <pthread.h>
#include "utils.h"
#include "players.h"

#define MUTEX_CHUNK_W 16
#define MUTEX_CHUNK_H 16
#define MAX_TILE_DEPTH 24
#define NOITEM 0xFF
#define ITEMID_BAG 0xFE
#define ITEMID_CRATE 0xFD
#define ITEMID_BLOCK 0x1
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
#define TICKS_FOR_BAG 450

typedef struct {
	unsigned char type;
	float start_x;
	float start_y;
	float angle;
	float x;
	float y;
	float r;
	Player* owner;
} Bullet;

typedef struct {
	unsigned char items[MAX_TILE_DEPTH];
	unsigned char count;
} Tile;

pthread_mutex_t** chunk_locks;
Tile** map;
unsigned int MapWidth;
unsigned int MapHeight;

#ifdef _USELIGHTMAP_
	unsigned char** LightMap;
#endif

Bullet bullets[MAX_BULLETS];
unsigned int bullet_id;
pthread_mutex_t bullet_lock;

unsigned long game_ticks;

Tile** CreateMap(unsigned int map_width, unsigned int map_height);
void DestroyMap(Tile** tiles, pthread_mutex_t** chunklocks);
void ReadMap(Tile*** dst, const char* name);
Tile* GetTile(unsigned int x, unsigned int y);
short PushTile(Tile* tile, char item);
unsigned char PopTile(Tile* tile);
unsigned char TopTile(Tile* tile);
signed char IsBlockingXY(unsigned short, unsigned short);
void InitBullets();
void CreateBullet(float, float, float, Player*);
void StartGame(Thread*);
void DumpMap(int,int,int,int);

void LockChunk(unsigned int x, unsigned int y);
void UnlockChunk(unsigned int x, unsigned int y);


#endif
