#include "../../include/global.h"

// Simple internal definitions for two biomes
static TileVariant desert_tiles[] = {
    { TILE_SAND, 0.7f },
    { TILE_ROCK, 0.2f },
    { TILE_CACTUS, 0.1f }
};

static TileVariant forest_tiles[] = {
    { TILE_GRASS, 0.6f },
    { TILE_TREE, 0.3f },
    { TILE_ROCK, 0.1f }
};

TileVariant get_tile_variant_for_biome(BiomeType biome, int x, int y) {
    float r = noise2d(x * 0.2f, y * 0.2f);
    TileVariant* variants = NULL;
    int count = 0;

    switch (biome) {
        case BIOME_DESERT:
            variants = desert_tiles;
            count = sizeof(desert_tiles) / sizeof(TileVariant);
            break;
        case BIOME_FOREST:
            variants = forest_tiles;
            count = sizeof(forest_tiles) / sizeof(TileVariant);
            break;
        default:
            return (TileVariant){ TILE_GRASS, 1.0f };
    }

    float total = 0.0f;
    for (int i = 0; i < count; i++) {
        total += variants[i].chance;
        if (r <= total)
            return variants[i];
    }
    return variants[count - 1];
}

BiomeType get_biome_from_noise(float elevation, float humidity) {
    if (elevation < 0.3f) return BIOME_WATER;
    if (elevation < 0.5f) {
        if (humidity > 0.6f) return BIOME_FOREST;
        return BIOME_PLAINS;
    }
    if (elevation < 0.7f) return BIOME_DESERT;
    if (elevation < 0.85f) return BIOME_MOUNTAIN;
    return BIOME_SNOW;
}

const char* biome_to_string(BiomeType biome) {
    switch (biome) {
        case BIOME_FOREST: return "Forest";
        case BIOME_DESERT: return "Desert";
        case BIOME_MOUNTAIN: return "Mountain";
        case BIOME_WATER: return "Water";
        case BIOME_SNOW: return "Snow";
        case BIOME_PLAINS: return "Plains";
        case BIOME_SWAMP: return "Swamp";
        case BIOME_JUNGLE: return "Jungle";
        case BIOME_VOLCANIC: return "Volcanic";
        case BIOME_CLOUD: return "Cloud";
        case BIOME_HEAVEN: return "Heaven";
        case BIOME_UNDERWORLD: return "Underworld";
        case BIOME_SACRED: return "Sacred";
        default: return "Unknown";
    }
}