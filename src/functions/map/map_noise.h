// Noise generation helpers for map creation.
#ifndef MAP_NOISE_H
#define MAP_NOISE_H

#include "../../include/structure.h"

// Retourne une hauteur normalisée [0, 1] à une position (x, y)
float get_height(int x, int y);

// Donne les données bruitées utiles à la classification
NoiseSample get_noise_sample(int x, int y);

// Calcule le biome à partir des données bruitées combinées
BiomeType get_biome_from_sample(NoiseSample sample);

// Retourne le biome associé à la position (x, y)
BiomeType get_biome_at(int x, int y);

float fbm(float x, float y, int octaves, float persistence, float scale);

float noise2d(float x, float y);

#endif

