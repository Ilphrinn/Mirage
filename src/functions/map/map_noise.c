#include "../../include/global.h"

float noise2d(float x, float y) {
    (void)x; (void)y;
    return 0.0f;
}

float fbm(float x, float y, int octaves, float persistence, float scale) {
    (void)x; (void)y; (void)octaves; (void)persistence; (void)scale;
    return 0.0f;
}

float get_height(int x, int y) {
    (void)x; (void)y;
    return 0.0f;
}

NoiseSample get_noise_sample(int x, int y) {
    NoiseSample sample = {0};
    sample.x = (float)x;
    sample.y = (float)y;
    sample.elevation = 0.0f;
    sample.humidity = 0.0f;
    sample.temperature = 0.0f;
    return sample;
}

BiomeType get_biome_from_sample(NoiseSample sample) {
    (void)sample;
    return BIOME_PLAINS;
}

BiomeType get_biome_at(int x, int y) {
    (void)x; (void)y;
    return BIOME_PLAINS;
}
