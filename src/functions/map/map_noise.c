#include <math.h>
#include <stdlib.h>
#include "../../include/global.h"

// --- Bruit de base (comme avant) ---
static float random2d(int x, int y) {
    int n = x * 374761393 + y * 668265263;
    n = (n ^ (n >> 13)) * 1274126177;
    return (float)((n ^ (n >> 16)) & 0x7FFFFFFF) / 2147483647.0f;
}

static float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

static float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float noise2d(float x, float y) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = fade(x - x0);
    float sy = fade(y - y0);

    float n00 = random2d(x0, y0);
    float n10 = random2d(x1, y0);
    float n01 = random2d(x0, y1);
    float n11 = random2d(x1, y1);

    float ix0 = lerp(n00, n10, sx);
    float ix1 = lerp(n01, n11, sx);
    return lerp(ix0, ix1, sy);
}

float fbm(float x, float y, int octaves, float persistence, float scale) {
    float total = 0.0f;
    float amplitude = 1.0f;
    float max_value = 0.0f;

    for (int i = 0; i < octaves; i++) {
        total += noise2d(x * scale, y * scale) * amplitude;
        max_value += amplitude;

        amplitude *= persistence;
        scale *= 2.0f;
    }

    return total / max_value;
}

NoiseSample get_noise_sample(int x, int y) {
    NoiseSample sample;
    sample.x = x;
    sample.y = y;
    sample.elevation = fbm(x, y, 5, 0.5f, 0.01f);  // ou autre usage
    sample.humidity = noise2d(x * 0.05f, y * 0.05f);
    sample.temperature = noise2d((x + 5000) * 0.05f, (y + 5000) * 0.05f);
    return sample;
}

BiomeType get_biome_from_sample(NoiseSample sample) {
    // Utiliser fBM sur la position pour une transition douce entre biomes
    float val = fbm(sample.x, sample.y, 5, 0.5f, 0.01f);

    if (val < 0.25f) return BIOME_DESERT;
    if (val < 0.5f)  return BIOME_PLAINS;
    if (val < 0.75f) return BIOME_FOREST;
    return BIOME_SNOW;
}

// --- Interface principale ---
float get_height(int x, int y) {
    return fbm((float)x / 50.0f, (float)y / 50.0f, 5, 0.5f, 0.01f);
}

BiomeType get_biome_at(int x, int y) {
    NoiseSample sample = get_noise_sample(x, y);
    return get_biome_from_sample(sample);
}