#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../../include/structure.h"

// Affiche les FPS en haut à gauche de l'écran
void render_fps(SDL_Renderer* renderer, TTF_Font* font, int fps);
void render_minimap(SDL_Renderer* renderer, float player_pos_x, float player_pos_y);
void render_debug_terminal(SDL_Renderer* renderer, TTF_Font* font, int screen_w, int screen_h, int cam_x, int cam_y);

const char* biome_to_string(BiomeType biome);

void set_tile_color(SDL_Renderer* r, TileID tile);

#endif