#ifndef ELEMENTS
#define ELEMENTS

#include "../../include/structure.h"

TileVariant get_tile_variant_for_biome(BiomeType biome, int x, int y);

BiomeType get_biome_from_noise(float elevation, float humidity);

const char* biome_to_string(BiomeType biome);

#endif