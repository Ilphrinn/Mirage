#include "../../include/global.h"

static unsigned int usage_counter = 0;
static Chunk chunk_cache[CHUNK_CACHE_SIZE];

const Chunk* get_chunk_cache(void) {
    return chunk_cache;
}

void map_init(void) {
    for (int i = 0; i < CHUNK_CACHE_SIZE; i++) {
        chunk_cache[i].loaded = false;
        chunk_cache[i].last_used = 0;
    }
    usage_counter = 0;
}

bool generate_chunk(TileID tiles[CHUNK_SIZE][CHUNK_SIZE], int chunk_x, int chunk_y) {
    int base_x = chunk_x * CHUNK_SIZE;
    int base_y = chunk_y * CHUNK_SIZE;

    if (base_x >= MAP_WIDTH || base_y >= MAP_HEIGHT)
        return false;

    // Génération des tuiles du chunk avec le nouveau système de biome
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int global_x = base_x + x;
            int global_y = base_y + y;

            BiomeType biome = get_biome_at(global_x, global_y);
            TileVariant variant = get_tile_variant_for_biome(biome, global_x, global_y);
            tiles[y][x] = variant.tile_id;
        }
    }

    return true;
}

TileID get_tile_at_cached(int global_x, int global_y) {
    int chunk_x = global_x / CHUNK_SIZE;
    int chunk_y = global_y / CHUNK_SIZE;
    int local_x = global_x % CHUNK_SIZE;
    int local_y = global_y % CHUNK_SIZE;

    for (int i = 0; i < CHUNK_CACHE_SIZE; i++) {
        if (chunk_cache[i].loaded &&
            chunk_cache[i].chunk_x == chunk_x &&
            chunk_cache[i].chunk_y == chunk_y) {
            chunk_cache[i].last_used = ++usage_counter;
            return chunk_cache[i].tiles[local_y][local_x];
        }
    }
    return 0;
}

TileID get_tile_at(int global_x, int global_y) {
    int chunk_x = global_x / CHUNK_SIZE;
    int chunk_y = global_y / CHUNK_SIZE;
    int local_x = global_x % CHUNK_SIZE;
    int local_y = global_y % CHUNK_SIZE;

    for (int i = 0; i < CHUNK_CACHE_SIZE; i++) {
        if (chunk_cache[i].loaded &&
            chunk_cache[i].chunk_x == chunk_x &&
            chunk_cache[i].chunk_y == chunk_y) {
            chunk_cache[i].last_used = ++usage_counter;
            return chunk_cache[i].tiles[local_y][local_x];
        }
    }

    int lru_index = -1;
    unsigned int oldest_use = ~0u;
    for (int i = 0; i < CHUNK_CACHE_SIZE; i++) {
        if (!chunk_cache[i].loaded) {
            lru_index = i;
            break;
        } else if (chunk_cache[i].last_used < oldest_use) {
            oldest_use = chunk_cache[i].last_used;
            lru_index = i;
        }
    }

    if (lru_index >= 0 && generate_chunk(chunk_cache[lru_index].tiles, chunk_x, chunk_y)) {
        chunk_cache[lru_index].chunk_x = chunk_x;
        chunk_cache[lru_index].chunk_y = chunk_y;
        chunk_cache[lru_index].loaded = true;
        chunk_cache[lru_index].last_used = ++usage_counter;
        return chunk_cache[lru_index].tiles[local_y][local_x];
    }

    return 0;
}

void Load_Chunks(int cam_tile_x, int cam_tile_y) {
    int cam_chunk_x = cam_tile_x / CHUNK_SIZE;
    int cam_chunk_y = cam_tile_y / CHUNK_SIZE;

    for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; dy++) {
        for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; dx++) {
            int target_chunk_x = cam_chunk_x + dx;
            int target_chunk_y = cam_chunk_y + dy;

            // Vérifie si le chunk est déjà chargé
            bool found = false;
            for (int i = 0; i < CHUNK_CACHE_SIZE; i++) {
                if (chunk_cache[i].loaded &&
                    chunk_cache[i].chunk_x == target_chunk_x &&
                    chunk_cache[i].chunk_y == target_chunk_y) {
                    chunk_cache[i].last_used = ++usage_counter;
                    found = true;
                    break;
                }
            }

            // Sinon, charge un nouveau chunk à cette position
            if (!found) {
                int lru_index = -1;
                unsigned int oldest_use = ~0u;
                for (int i = 0; i < CHUNK_CACHE_SIZE; i++) {
                    if (!chunk_cache[i].loaded) {
                        lru_index = i;
                        break;
                    } else if (chunk_cache[i].last_used < oldest_use) {
                        oldest_use = chunk_cache[i].last_used;
                        lru_index = i;
                    }
                }

                if (lru_index >= 0) {
                    if (generate_chunk(chunk_cache[lru_index].tiles, target_chunk_x, target_chunk_y)) {
                        chunk_cache[lru_index].chunk_x = target_chunk_x;
                        chunk_cache[lru_index].chunk_y = target_chunk_y;
                        chunk_cache[lru_index].loaded = true;
                        chunk_cache[lru_index].last_used = ++usage_counter;
                    }
                }
            }
        }
    }
}

int get_chunk_count(void) {
    int count = 0;
    for (int i = 0; i < CHUNK_CACHE_SIZE; ++i) {
        if (chunk_cache[i].loaded) count++;
    }
    return count;
}