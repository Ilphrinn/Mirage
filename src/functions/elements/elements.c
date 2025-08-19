#include "../../include/global.h"

TileVariant get_tile_variant_for_biome(BiomeType biome, int x, int y) {
    (void)biome; (void)x; (void)y;
    return (TileVariant){TILE_GRASS, 1.0f};
}

BiomeType get_biome_from_noise(float elevation, float humidity) {
    (void)elevation; (void)humidity;
    return BIOME_PLAINS;
}

const char* biome_to_string(BiomeType biome) {
    switch (biome) {
        case BIOME_PLAINS: return "Plains";
        default: return "Plains";
    }
}
