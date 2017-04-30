#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <math.h>

#include "game.h"
#include "utils.h"

Tile** CreateMap(unsigned int map_width, unsigned int map_height) {
	Tile** tiles = (Tile**)malloc(map_height*sizeof(Tile*));
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
    return tiles;
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

void ReadMap(Tile*** dst, const char* name){
    short width,height;
    FILE* mapfile = fopen(name,"r");
    fread(&width,2,1,mapfile);
    width = SwapShort(width);
    fread(&height,2,1,mapfile);
    height = SwapShort(height);
    printf(CLR_C"Creating map %dx%d\n"CLR_N,width,height);

    *dst = CreateMap(width,height);
    short x, y, z; unsigned char readTile;
    for(y=0;y<height;y++)
        for(x=0;x<width;x++)
            for(z=0;z<24;z++) {
                fread(&readTile,sizeof(char),1,mapfile);
                (*dst)[y][x].items[z]=readTile;
                if(readTile!=0) (*dst)[y][x].count=z+1;
            }
    fclose(mapfile);
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
	int i; unsigned char item;
	item = 0;
	for(i=0;i<tile->count;++i) {
		item = tile->items[i];
		if(item==7 || item>=12)
		#ifdef _HARDDEBUG_
			{printf(CLR_Y"Blocking by item %d\n"CLR_N,item); return 1;}
		#else
			return 1;
		#endif
	}
	return 0;
}

signed char IsBlockingXY(unsigned short x, unsigned short y) {
	if(x<0 || y<0 || x>=MapWidth || y>=MapHeight) return 1;
	return IsBlocking(&map[y][x]);
}

void UpdateBullets() {
	int i; i=0;
	pthread_mutex_lock(&bullet_lock);
	Bullet* b;
	for(;i<MAX_BULLETS;++i) {
		b=&bullets[i];
		if(b->type>0) {
			switch(b->type) {
				case BULLET_EARTH: b->r+=BULLET_EARTH_SPEED; if(b->r>BULLET_EARTH_DISTANCE) b->type=0; break;
				case BULLET_FIRE: b->r+=BULLET_FIRE_SPEED; if(b->r>BULLET_FIRE_DISTANCE) b->type=0; break;
				case BULLET_KNIFE: b->r+=BULLET_KNIFE_SPEED; if(b->r>BULLET_KNIFE_DISTANCE) b->type=0; break;
			}
			b->x=b->start_x+b->r*cos(b->angle);
			b->y=b->start_y+b->r*sin(b->angle);
			//if(IsBlockingXY((int)roundf(b->x),(int)roundf(b->y))) b->type=0;
		}
	}
	pthread_mutex_unlock(&bullet_lock);
}

void InitBullets() {
	int i;
	for(i=0;i<MAX_BULLETS;++i) {
		bullets[i].type=BULLET_EMPTY;
	}	
}

void CreateBullet(float x, float y, float angle) {
	pthread_mutex_lock(&bullet_lock);
	bullets[bullet_id].angle=angle;
	bullets[bullet_id].x=x;
	bullets[bullet_id].y=y;
	bullets[bullet_id].start_x=x;
	bullets[bullet_id].start_y=y;
	bullets[bullet_id].r=0;
	bullets[bullet_id].type=BULLET_FIRE;
	bullet_id++;
	bullet_id=bullet_id%MAX_BULLETS;
	pthread_mutex_unlock(&bullet_lock);
}

void* GameFunction(void* arg) {
	#ifdef _DEBUG_
		printf(CLR_C"Game:"CLR_N" Starting...\n");
	#endif
	
	Thread* this = arg;
	
	struct timeval tv;
	unsigned long sleep_u;
	
	while(this->alive == THREAD_ALIVE) {
		Tick(&tv,&sleep_u);
		
			UpdateBullets();
		
		sleep_u=Tock(&tv,&sleep_u);
		usleep(25000-sleep_u);
	}
	
	#ifdef _DEBUG_
		printf(CLR_Y"Game:"CLR_N" Exiting...\n");
	#endif
	pthread_exit(NULL);
}

void StartGame(Thread* t) {
	t->alive = THREAD_ALIVE;
	pthread_t* tmp = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(tmp, NULL, GameFunction, t);
	t->self = tmp;
}

void DumpMap(int dump_x, int dump_y, int dump_w, int dump_h) {
	printf("=== %d , %d ===\n",dump_x,dump_y);
	for(int dump_j=dump_y;dump_j<dump_y+dump_h;++dump_j) {
		if(dump_j<0 || dump_j>=MapHeight) { continue;}
		for(int dump_i=dump_x;dump_i<dump_x+dump_w;++dump_i) {
			if(dump_i<0 || dump_i>=MapWidth) {printf(","); continue;}
			for(int dump_k=0;dump_k<map[dump_j][dump_i].count;++dump_k)
				printf("%d ",map[dump_j][dump_i].items[dump_k]);
			printf(",");
			}
		printf("\n");
	}
}
