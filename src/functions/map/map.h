#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../../include/structure.h"

// --- Configuration ---
#define LOAD_RADIUS 3
#define LOAD_DIAMETER (2 * LOAD_RADIUS + 1)

#define MAP_WIDTH 100000000LL
#define MAP_HEIGHT 100000000LL

#define MAP_PIXEL_WIDTH  ((int64_t)(MAP_WIDTH * TILE_SIZE))
#define MAP_PIXEL_HEIGHT ((int64_t)(MAP_HEIGHT * TILE_SIZE))

#define TILE_SIZE        32  // in pixels
#define CHUNK_SIZE       64  // in tiles (64x64)
#define CHUNK_BYTES      (CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_CACHE_SIZE (LOAD_DIAMETER * LOAD_DIAMETER)

#define TILE_MAX TILE_COUNT

// --- Cache initialization ---
void map_init(void);

// --- Access to a global tile (coordinates in tiles, not pixels) ---
TileID get_tile_at_cached(int global_x, int global_y);
TileID get_tile_at(int global_x, int global_y);

void Load_Chunks(int cam_x, int cam_y);
void map_init(void);

typedef struct {
    int chunk_x;
    int chunk_y;
    TileID tiles[CHUNK_SIZE][CHUNK_SIZE];
    bool loaded;
    unsigned int last_used;
} Chunk;

const Chunk* get_chunk_cache(void);

bool generate_chunk(TileID tiles[CHUNK_SIZE][CHUNK_SIZE], int chunk_x, int chunk_y);

int get_chunk_count(void);

#endif