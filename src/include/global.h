#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// === SDL ===
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern bool debug_mode; // déclaration

// === Modules du jeu ===
#include "../functions/character/character.h"
#include "../functions/floor/floor.h"

#include "../functions/elements/elements.h"

#include "../functions/map/map.h"
#include "../functions/map/map_noise.h"

#include "../functions/ui/ui.h"

#include "../functions/debug/debug.h"

#endif
