#include <stdlib.h>
#include <pthread.h>

#include "game.h"

void CreateMap(Tile** tiles, unsigned int map_width, unsigned int map_height) {
	tiles = (Tile**)malloc(map_height*sizeof(Tile*));
	unsigned int y,x;
	for(y=0;y<map_height;++y){
		tiles[y] = (Tile*)malloc(map_width*sizeof(Tile));
		for(x=0;x<map_width;++x)
			tiles[y][x].count = 0;
	}
	
	MapWidth = map_width;
	MapHeight = map_height;
	
	unsigned short locks_w = map_width/MUTEX_CHUNK_W+1;
	unsigned short locks_h = map_height/MUTEX_CHUNK_H+1;
	chunk_locks = (pthread_mutex_t**)malloc(locks_h*sizeof(pthread_mutex_t*));
	for(y=0;y<locks_h;++y)
		chunk_locks[y] = (pthread_mutex_t*)malloc(locks_w*sizeof(pthread_mutex_t));
}

void DestroyMap(Tile** tiles, pthread_mutex_t** chunklocks) {
	unsigned int y;
	for(y=0;y<MapHeight;++y)
		free(tiles[y]);
	free(tiles);
	unsigned short locks_h = MapHeight/MUTEX_CHUNK_H+1;
	for(y=0;y<locks_h;++y)
		free(chunklocks[y]);
	free(chunklocks);
}

void LockChunk(unsigned int x, unsigned int y) {
	unsigned int lock_x = x/MUTEX_CHUNK_W;
	unsigned int lock_y = y/MUTEX_CHUNK_H;
	pthread_mutex_lock(&chunk_locks[lock_y][lock_x]);
}

void UnlockChunk(unsigned int x, unsigned int y) {
	unsigned int lock_x = x/MUTEX_CHUNK_W;
	unsigned int lock_y = y/MUTEX_CHUNK_H;
	pthread_mutex_unlock(&chunk_locks[lock_y][lock_x]);
}

short PushTile(Tile* tile, char item) {
	if(tile->count<MAX_TILE_DEPTH-1) {
		tile->items[tile->count]=item;
		++tile->count;
		return ITEM_PUSHED;
	}
	
	return NO_TILE_SPACE;
}

unsigned char PopTile(Tile* tile) {
	if(tile->count<=0)
		return NOITEM;
	--tile->count;
	return tile->items[tile->count];
}

unsigned char TopTile(Tile* tile) {
	if(tile->count<=0)
		return NOITEM;
	return tile->items[tile->count-1];
}

signed char IsBlocking(Tile* tile) {
	
}
