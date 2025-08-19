#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../../include/structure.h"

// Display FPS in the top-left corner of the screen
void render_fps(SDL_Renderer* renderer, TTF_Font* font, int debug_fps);
void render_minimap(SDL_Renderer* renderer, float debug_player_pos_x, float debug_player_pos_y);
void render_debug_terminal(SDL_Renderer* renderer, TTF_Font* font, int debug_screen_w, int debug_screen_h, int debug_cam_x, int debug_cam_y);

const char* biome_to_string(BiomeType biome);

void set_tile_color(SDL_Renderer* r, TileID tile);
void set_biome_color(SDL_Renderer* r, BiomeType biome);

#endif