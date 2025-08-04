#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include "../../include/structure.h"

Player create_player(int x, int y, int size, int speed);
void move_player(Player* player, const Uint8* keystates);
void update_camera(SDL_Rect* camera, const Player* player);
void render_player(SDL_Renderer* renderer, const Player* player, const SDL_Rect* camera);

#endif // CHARACTER_H