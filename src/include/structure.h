#ifndef STRUCTURE
#define STRUCTURE

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdint.h>

// === Player ===
typedef struct {
    SDL_Rect rect;
    SDL_Rect hitbox;
    int speed;
    SDL_Color color;
    int64_t pos_x;
    int64_t pos_y;
} Player;

// === Render groups ===
typedef struct {
    SDL_Rect rects[2048];
    int count;
    SDL_Color color;
} TileRenderGroup;

// === Tile with weight for weighted selection ===
typedef struct {
    int tile_id;
    float chance;
} TileVariant;

// === Noise data sample ===
typedef struct {
    float x, y;  // new
    float elevation;
    float humidity;
    float temperature;
} NoiseSample;

// === Tile types ===
typedef enum {
    // --- Natural ground ---
    TILE_GRASS = 0,
    TILE_SAND,
    TILE_ROCK,
    TILE_SNOW,
    TILE_WATER,
    TILE_MUD,
    TILE_SOIL,
    TILE_RICE_FIELD,
    TILE_BAMBOO_FLOOR,

    // --- Vegetation ---
    TILE_TREE,
    TILE_PINE_TREE,
    TILE_BAMBOO,
    TILE_BUSH,
    TILE_CACTUS,
    TILE_FLOWER,
    TILE_MUSHROOM,

    // --- Natural formations ---
    TILE_MOUNTAIN,
    TILE_HILL,
    TILE_CLIFF,
    TILE_CAVE,
    TILE_HOT_SPRING,
    TILE_RIVER,
    TILE_LAKE,

    // --- Human structures ---
    TILE_WALL,
    TILE_PATH,
    TILE_FLOOR,
    TILE_BRICK,
    TILE_ROOF,
    TILE_BRIDGE,
    TILE_TEMPLE,
    TILE_PAGODA,
    TILE_HOUSE,
    TILE_PALACE,
    TILE_MARKET,
    TILE_GATE,

    // --- Sacred symbols/objects ---
    TILE_SHRINE,
    TILE_ALTAR,
    TILE_STATUE,
    TILE_DRAGON_TOTEM,
    TILE_LANTERN,
    TILE_SPIRIT_TREE,

    // --- Special areas ---
    TILE_OASIS,
    TILE_LAVA,
    TILE_ICE,
    TILE_MIST,
    TILE_CLOUD,
    TILE_VOID,

    // --- Miscellaneous / technical ---
    TILE_PORTAL,
    TILE_CHECKPOINT,
    TILE_SECRET_DOOR,
    TILE_TRIGGER,

    // --- Fallback ---
    TILE_UNKNOWN,


    // --- VALUE, DO NOT MOVE ---
    TILE_COUNT
} TileID;

static SDL_Color tile_colors[TILE_COUNT] = {
    [TILE_GRASS]        = { 34, 139,  34, 255 },
    [TILE_SAND]         = {237, 201, 175, 255 },
    [TILE_ROCK]         = {112, 128, 144, 255 },
    [TILE_SNOW]         = {255, 250, 250, 255 },
    [TILE_WATER]        = { 30, 144, 255, 255 },
    [TILE_MUD]          = {102,  51,   0, 255 },
    [TILE_SOIL]         = {139,  69,  19, 255 },
    [TILE_RICE_FIELD]   = {170, 255, 100, 255 },
    [TILE_BAMBOO_FLOOR] = {194, 178, 128, 255 },

    [TILE_TREE]         = {  0, 100,   0, 255 },
    [TILE_PINE_TREE]    = {  1,  68,  33, 255 },
    [TILE_BAMBOO]       = {152, 251, 152, 255 },
    [TILE_BUSH]         = { 34, 139,  34, 255 },
    [TILE_CACTUS]       = {  0, 128,   0, 255 },
    [TILE_FLOWER]       = {255, 105, 180, 255 },
    [TILE_MUSHROOM]     = {178,  34,  34, 255 },

    [TILE_MOUNTAIN]     = {139, 137, 137, 255 },
    [TILE_HILL]         = {160,  82,  45, 255 },
    [TILE_CLIFF]        = {105, 105, 105, 255 },
    [TILE_CAVE]         = { 47,  79,  79, 255 },
    [TILE_HOT_SPRING]   = {  0, 206, 209, 255 },
    [TILE_RIVER]        = {  0, 191, 255, 255 },
    [TILE_LAKE]         = { 70, 130, 180, 255 },

    [TILE_WALL]         = {128, 128, 128, 255 },
    [TILE_PATH]         = {160, 160, 160, 255 },
    [TILE_FLOOR]        = {192, 192, 192, 255 },
    [TILE_BRICK]        = {178,  34,  34, 255 },
    [TILE_ROOF]         = {139,   0,   0, 255 },
    [TILE_BRIDGE]       = {205, 133,  63, 255 },
    [TILE_TEMPLE]       = {184, 134,  11, 255 },
    [TILE_PAGODA]       = {160,  82,  45, 255 },
    [TILE_HOUSE]        = {205,  92,  92, 255 },
    [TILE_PALACE]       = {218, 165,  32, 255 },
    [TILE_MARKET]       = {255, 140,   0, 255 },
    [TILE_GATE]         = {178,  34,  34, 255 },

    [TILE_SHRINE]       = {186,  85, 211, 255 },
    [TILE_ALTAR]        = {147, 112, 219, 255 },
    [TILE_STATUE]       = {169, 169, 169, 255 },
    [TILE_DRAGON_TOTEM] = {255,  69,   0, 255 },
    [TILE_LANTERN]      = {255, 215,   0, 255 },
    [TILE_SPIRIT_TREE]  = {144, 238, 144, 255 },

    [TILE_OASIS]        = {135, 206, 235, 255 },
    [TILE_LAVA]         = {255,  69,   0, 255 },
    [TILE_ICE]          = {173, 216, 230, 255 },
    [TILE_MIST]         = {211, 211, 211, 255 },
    [TILE_CLOUD]        = {245, 245, 245, 255 },
    [TILE_VOID]         = { 25,  25, 112, 255 },

    [TILE_PORTAL]       = {123, 104, 238, 255 },
    [TILE_CHECKPOINT]   = {255, 215,   0, 255 },
    [TILE_SECRET_DOOR]  = {139,   0, 139, 255 },
    [TILE_TRIGGER]      = {255,  20, 147, 255 },

    [TILE_UNKNOWN]      = { 60,  60,  60, 255 }
};

// === Biome types ===
typedef enum {
    BIOME_FOREST,
    BIOME_DESERT,
    BIOME_MOUNTAIN,
    BIOME_WATER,
    BIOME_SNOW,
    BIOME_PLAINS,
    BIOME_SWAMP,
    BIOME_JUNGLE,
    BIOME_VOLCANIC,
    BIOME_CLOUD,
    BIOME_HEAVEN,
    BIOME_UNDERWORLD,
    BIOME_SACRED,
    BIOME_UNKNOWN
} BiomeType;

#define BIOME_COUNT (BIOME_UNKNOWN + 1)

static SDL_Color biome_colors[BIOME_COUNT] = {
    [BIOME_FOREST]     = { 34, 139,  34, 255 },
    [BIOME_DESERT]     = {237, 201, 175, 255 },
    [BIOME_MOUNTAIN]   = {139, 137, 137, 255 },
    [BIOME_WATER]      = { 30, 144, 255, 255 },
    [BIOME_SNOW]       = {255, 250, 250, 255 },
    [BIOME_PLAINS]     = {144, 238, 144, 255 },
    [BIOME_SWAMP]      = { 47,  79,  79, 255 },
    [BIOME_JUNGLE]     = {  0, 100,   0, 255 },
    [BIOME_VOLCANIC]   = {178,  34,  34, 255 },
    [BIOME_CLOUD]      = {245, 245, 245, 255 },
    [BIOME_HEAVEN]     = {255, 255, 224, 255 },
    [BIOME_UNDERWORLD] = {105,   0, 105, 255 },
    [BIOME_SACRED]     = {186,  85, 211, 255 },
    [BIOME_UNKNOWN]    = { 60,  60,  60, 255 }
};

#endif
