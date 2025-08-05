// Implementation of 2D Perlin noise with fBm helpers.
#include <math.h>
#include <stdlib.h>
#include "../../include/global.h"

static const unsigned char base_perm[256] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,
    69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,
    252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,
    168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,
    211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,
    216,80,73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,
    164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,
    126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,
    213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,
    253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,
    242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,
    192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

static unsigned char perm[512];
static int perm_initialized = 0;

static void init_perm(void) {
    for (int i = 0; i < 256; ++i) {
        perm[i] = perm[256 + i] = base_perm[i];
    }
    perm_initialized = 1;
}

static float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

static float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static float grad(int hash, float x, float y) {
    switch (hash & 7) {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        case 3: return -x - y;
        case 4: return  x;
        case 5: return -x;
        case 6: return  y;
        default: return -y;
    }
}

float noise2d(float x, float y) {
    if (!perm_initialized) init_perm();

    int X = (int)floorf(x) & 255;
    int Y = (int)floorf(y) & 255;

    x -= floorf(x);
    y -= floorf(y);

    float u = fade(x);
    float v = fade(y);

    int A = perm[X] + Y;
    int B = perm[X + 1] + Y;

    float res = lerp(
        lerp(grad(perm[A], x, y),
             grad(perm[B], x - 1, y), u),
        lerp(grad(perm[A + 1], x, y - 1),
             grad(perm[B + 1], x - 1, y - 1), u),
        v);

    return (res + 1.0f) * 0.5f; // normalisé [0,1]
}

float fbm(float x, float y, int octaves, float persistence, float scale) {
    float total = 0.0f;
    float amplitude = 1.0f;
    float max_value = 0.0f;
    float frequency = scale;

    for (int i = 0; i < octaves; i++) {
        total += noise2d(x * frequency, y * frequency) * amplitude;
        max_value += amplitude;

        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return max_value > 0.0f ? total / max_value : 0.0f;
}

NoiseSample get_noise_sample(int x, int y) {
    NoiseSample sample;
    sample.x = (float)x;
    sample.y = (float)y;
    sample.elevation = fbm((float)x, (float)y, 5, 0.5f, 0.01f);
    sample.humidity = noise2d(x * 0.05f, y * 0.05f);
    sample.temperature = noise2d((x + 5000) * 0.05f, (y + 5000) * 0.05f);
    return sample;
}

BiomeType get_biome_from_sample(NoiseSample sample) {
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
