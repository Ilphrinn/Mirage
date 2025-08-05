// Noise generation helpers for map creation.
#ifndef MAP_NOISE_H
#define MAP_NOISE_H

#include "../../include/structure.h"

// Returns a normalized height [0, 1] at position (x, y)
float get_height(int x, int y);

// Provides noise data useful for classification
NoiseSample get_noise_sample(int x, int y);

// Computes the biome from combined noise data
BiomeType get_biome_from_sample(NoiseSample sample);

// Returns the biome associated with position (x, y)
BiomeType get_biome_at(int x, int y);

float fbm(float x, float y, int octaves, float persistence, float scale);

float noise2d(float x, float y);

#endif

