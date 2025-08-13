#include "../../include/global.h"

void render_fps(SDL_Renderer* renderer, TTF_Font* font, int fps) {
    // Static variables to cache the texture
    static int last_fps = -1;
    static SDL_Texture* fpsTexture = NULL;
    static char cachedText[32] = "";
    SDL_Color white = {255, 255, 255, 255};

    // Update the texture only if the FPS has changed
    if (fps != last_fps) {
        last_fps = fps;
        snprintf(cachedText, sizeof(cachedText), "FPS: %d", fps);
        if (fpsTexture) {
            SDL_DestroyTexture(fpsTexture);
            fpsTexture = NULL;
        }
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, cachedText, white);
        if (surface) {
            fpsTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }

    // Render the texture if it is available
    if (fpsTexture) {
        SDL_Rect dst;
        SDL_QueryTexture(fpsTexture, NULL, NULL, &dst.w, &dst.h);
        int win_w, win_h;
        SDL_GetRendererOutputSize(renderer, &win_w, &win_h);
        dst.x = win_w - dst.w - 10; // margin of 10 pixels
        dst.y = 10;
        SDL_RenderCopy(renderer, fpsTexture, NULL, &dst);
    }

}

void render_minimap(SDL_Renderer* renderer, float player_pos_x, float player_pos_y) {
    const Chunk* chunk_cache = get_chunk_cache();
    const int TILE_PIXEL = 2;
    const int MARGIN_X = 10;
    const int MARGIN_Y = 10;
    const int RADIUS_TILES = 50;

    int center_tile_x = (int)(player_pos_x / TILE_SIZE);
    int center_tile_y = (int)(player_pos_y / TILE_SIZE);

    // black background with border
    SDL_Rect bg = {
        MARGIN_X - 1,
        MARGIN_Y - 1,
        (2 * RADIUS_TILES + 1) * TILE_PIXEL + 2,
        (2 * RADIUS_TILES + 1) * TILE_PIXEL + 2
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bg);
    SDL_RenderDrawRect(renderer, &bg);

    for (int i = 0; i < CHUNK_CACHE_SIZE; i++) {
        if (!chunk_cache[i].loaded) continue;

        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                int global_x = chunk_cache[i].chunk_x * CHUNK_SIZE + x;
                int global_y = chunk_cache[i].chunk_y * CHUNK_SIZE + y;

                if (global_x < 0 || global_y < 0 || global_x >= MAP_WIDTH || global_y >= MAP_HEIGHT)
                    continue;

                int dx = global_x - center_tile_x;
                int dy = global_y - center_tile_y;
                if (dx < -RADIUS_TILES || dx > RADIUS_TILES || dy < -RADIUS_TILES || dy > RADIUS_TILES)
                    continue;

                SDL_Rect rect = {
                    MARGIN_X + (dx + RADIUS_TILES) * TILE_PIXEL,
                    MARGIN_Y + (dy + RADIUS_TILES) * TILE_PIXEL,
                    TILE_PIXEL,
                    TILE_PIXEL
                };

                BiomeType biome = get_biome_at(global_x, global_y);
                set_biome_color(renderer, biome);

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // Player marker at the center
    SDL_Rect marker = {
        MARGIN_X + RADIUS_TILES * TILE_PIXEL,
        MARGIN_Y + RADIUS_TILES * TILE_PIXEL,
        TILE_PIXEL,
        TILE_PIXEL
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &marker);
}

void set_tile_color(SDL_Renderer* r, TileID tile) {
    SDL_Color c = tile_colors[tile < TILE_COUNT ? tile : TILE_UNKNOWN];
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

void set_biome_color(SDL_Renderer* r, BiomeType biome) {
    SDL_Color c = biome_colors[biome < BIOME_COUNT ? biome : BIOME_UNKNOWN];
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

extern int debug_terminal_mode;
extern char debug_input[128];
extern int debug_input_len;

void render_debug_terminal(SDL_Renderer* renderer, TTF_Font* font, int w, int h, int cam_x, int cam_y) {
    SDL_Rect rect = {0, h - h / 4, w / 2, h / 4};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Rect log_rect = rect;
    log_rect.h -= 30;  // Leave room for input
    debug_render_in_rect(renderer, font, log_rect);

    SDL_Color color = {255, 255, 255, 255};
    char buffer[256];

    if (debug_terminal_mode) {
        snprintf(buffer, sizeof(buffer), "> %s", debug_input);
    } else {
        snprintf(buffer, sizeof(buffer), "Pos: (%d,%d)  Chunks: %d", cam_x, cam_y, get_chunk_count());
    }


    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, buffer, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect text_rect = {
        rect.x + 10,
        rect.y + rect.h - surf->h - 5,
        surf->w,
        surf->h
    };

    SDL_RenderCopy(renderer, tex, NULL, &text_rect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}