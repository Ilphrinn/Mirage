#include "../../include/global.h"

void render_fps(SDL_Renderer* renderer, TTF_Font* font, int debug_fps) {
    static int debug_last_fps = -1;
    static SDL_Texture* debug_fps_texture = NULL;
    static char debug_cached_text[32] = "";
    SDL_Color debug_white = {255, 255, 255, 255};

    if (debug_fps != debug_last_fps) {
        debug_last_fps = debug_fps;
        snprintf(debug_cached_text, sizeof(debug_cached_text), "FPS: %d", debug_fps);
        if (debug_fps_texture) {
            SDL_DestroyTexture(debug_fps_texture);
            debug_fps_texture = NULL;
        }
        SDL_Surface* debug_surface = TTF_RenderUTF8_Blended(font, debug_cached_text, debug_white);
        if (debug_surface) {
            debug_fps_texture = SDL_CreateTextureFromSurface(renderer, debug_surface);
            SDL_FreeSurface(debug_surface);
        }
    }

    if (debug_fps_texture) {
        SDL_Rect debug_dst;
        SDL_QueryTexture(debug_fps_texture, NULL, NULL, &debug_dst.w, &debug_dst.h);
        int debug_win_w, debug_win_h;
        SDL_GetRendererOutputSize(renderer, &debug_win_w, &debug_win_h);
        debug_dst.x = debug_win_w - debug_dst.w - 10;
        debug_dst.y = 10;
        SDL_RenderCopy(renderer, debug_fps_texture, NULL, &debug_dst);
    }
}

void render_minimap(SDL_Renderer* renderer, float debug_player_pos_x, float debug_player_pos_y) {
    const Chunk* debug_chunk_cache = get_chunk_cache();
    const int debug_tile_pixel = 2;
    const int debug_margin_x = 10;
    const int debug_margin_y = 10;
    const int debug_radius_tiles = 50;

    int debug_center_tile_x = (int)(debug_player_pos_x / TILE_SIZE);
    int debug_center_tile_y = (int)(debug_player_pos_y / TILE_SIZE);

    SDL_Rect debug_bg = {
        debug_margin_x - 1,
        debug_margin_y - 1,
        (2 * debug_radius_tiles + 1) * debug_tile_pixel + 2,
        (2 * debug_radius_tiles + 1) * debug_tile_pixel + 2
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &debug_bg);
    SDL_RenderDrawRect(renderer, &debug_bg);

    for (int debug_i = 0; debug_i < CHUNK_CACHE_SIZE; debug_i++) {
        if (!debug_chunk_cache[debug_i].loaded) continue;

        for (int debug_y = 0; debug_y < CHUNK_SIZE; debug_y++) {
            for (int debug_x = 0; debug_x < CHUNK_SIZE; debug_x++) {
                int global_x = debug_chunk_cache[debug_i].chunk_x * CHUNK_SIZE + debug_x;
                int global_y = debug_chunk_cache[debug_i].chunk_y * CHUNK_SIZE + debug_y;

                if (global_x < 0 || global_y < 0 || global_x >= MAP_WIDTH || global_y >= MAP_HEIGHT)
                    continue;

                int debug_dx = global_x - debug_center_tile_x;
                int debug_dy = global_y - debug_center_tile_y;
                if (debug_dx < -debug_radius_tiles || debug_dx > debug_radius_tiles ||
                    debug_dy < -debug_radius_tiles || debug_dy > debug_radius_tiles)
                    continue;

                SDL_Rect debug_rect = {
                    debug_margin_x + (debug_dx + debug_radius_tiles) * debug_tile_pixel,
                    debug_margin_y + (debug_dy + debug_radius_tiles) * debug_tile_pixel,
                    debug_tile_pixel,
                    debug_tile_pixel
                };

                BiomeType debug_biome = get_biome_at(global_x, global_y);
                set_biome_color(renderer, debug_biome);

                SDL_RenderFillRect(renderer, &debug_rect);
            }
        }
    }

    SDL_Rect debug_marker = {
        debug_margin_x + debug_radius_tiles * debug_tile_pixel,
        debug_margin_y + debug_radius_tiles * debug_tile_pixel,
        debug_tile_pixel,
        debug_tile_pixel
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &debug_marker);
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

void render_debug_terminal(SDL_Renderer* renderer, TTF_Font* font, int debug_w, int debug_h, int debug_cam_x, int debug_cam_y) {
    SDL_Rect debug_rect = {0, debug_h - debug_h / 4, debug_w / 2, debug_h / 4};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    SDL_RenderFillRect(renderer, &debug_rect);

    SDL_Rect debug_log_rect = debug_rect;
    debug_log_rect.h -= 30;
    debug_render_in_rect(renderer, font, debug_log_rect);

    SDL_Color debug_color = {255, 255, 255, 255};
    char debug_buffer[256];

    if (debug_terminal_mode) {
        snprintf(debug_buffer, sizeof(debug_buffer), "> %s", debug_input);
    } else {
        snprintf(debug_buffer, sizeof(debug_buffer), "Pos: (%d,%d)  Chunks: %d", debug_cam_x, debug_cam_y, get_chunk_count());
    }

    SDL_Surface* debug_surf = TTF_RenderUTF8_Blended(font, debug_buffer, debug_color);
    SDL_Texture* debug_tex = SDL_CreateTextureFromSurface(renderer, debug_surf);
    SDL_Rect debug_text_rect = {
        debug_rect.x + 10,
        debug_rect.y + debug_rect.h - debug_surf->h - 5,
        debug_surf->w,
        debug_surf->h
    };

    SDL_RenderCopy(renderer, debug_tex, NULL, &debug_text_rect);
    SDL_FreeSurface(debug_surf);
    SDL_DestroyTexture(debug_tex);
}
