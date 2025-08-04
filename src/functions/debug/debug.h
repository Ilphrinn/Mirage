// === SDL ===
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../../include/structure.h"

void debug_clear();

void debug_print(const char *fmt, ...);

void execute_debug_command(const char* cmd, Player* player);

void debug_render_in_rect(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect rect);

void debug_terminal_log(const char* fmt, ...);