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

typedef struct {
	unsigned char items[MAX_TILE_DEPTH];
	unsigned short count;
} Tile;

pthread_mutex_t** chunk_locks;
Tile** map;
unsigned int MapWidth;
unsigned int MapHeight;

void CreateMap(Tile** tiles, unsigned int map_width, unsigned int map_height);
short PushTile(Tile* tile, char item);
unsigned char PopTile(Tile* tile);
unsigned char TopTile(Tile* tile);

#endif
