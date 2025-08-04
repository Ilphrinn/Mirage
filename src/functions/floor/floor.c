
#include "../../include/global.h"
#include "../../include/structure.h"

void render_floor(SDL_Renderer* renderer, SDL_Rect camera) {
    const int start_x = camera.x / TILE_SIZE;
    const int end_x   = (camera.x + camera.w) / TILE_SIZE + 1;
    const int start_y = camera.y / TILE_SIZE;
    const int end_y   = (camera.y + camera.h) / TILE_SIZE + 1;


for (int y = start_y; y < end_y; ++y) {
    for (int x = start_x; x < end_x; ++x) {
        TileID tile = get_tile_at_cached(x, y);

        SDL_Rect rect = {
            x * TILE_SIZE - camera.x,
            y * TILE_SIZE - camera.y,
            TILE_SIZE,
            TILE_SIZE
        };

        SDL_Color c = tile_colors[tile < TILE_COUNT ? tile : TILE_UNKNOWN];
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(renderer, &rect);
    }
}

if (debug_mode) {
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    for (int x = start_x; x <= end_x; ++x) {
        int line_x = x * TILE_SIZE - camera.x;
        SDL_RenderDrawLine(renderer, line_x, 0, line_x, camera.h);
    }
    for (int y = start_y; y <= end_y; ++y) {
        int line_y = y * TILE_SIZE - camera.y;
        SDL_RenderDrawLine(renderer, 0, line_y, camera.w, line_y);
    }
}

}