#include "../../include/global.h"

bool debug_mode = false;

#define DEBUG_BUFFER_LINES 8

static char debug_lines[DEBUG_BUFFER_LINES][128]; // max 5 lines
static int debug_line_count = 0;

void debug_clear() {
    debug_line_count = 0;
}

void debug_terminal_log(const char* fmt, ...) {
    if (debug_line_count >= DEBUG_BUFFER_LINES) {
        // shift lines (scroll)
        for (int debug_i = 1; debug_i < DEBUG_BUFFER_LINES; debug_i++) {
            strcpy(debug_lines[debug_i - 1], debug_lines[debug_i]);
        }
        debug_line_count = DEBUG_BUFFER_LINES - 1;
    }

    va_list debug_args;
    va_start(debug_args, fmt);
    vsnprintf(debug_lines[debug_line_count++], 128, fmt, debug_args);
    va_end(debug_args);
}

void debug_render_in_rect(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect debug_rect) {
    SDL_Color debug_white = {255, 255, 255, 255};
    for (int debug_i = 0; debug_i < debug_line_count; debug_i++) {
        SDL_Surface *debug_surface = TTF_RenderUTF8_Blended(font, debug_lines[debug_i], debug_white);
        SDL_Texture *debug_texture = SDL_CreateTextureFromSurface(renderer, debug_surface);
        SDL_Rect debug_dst = {
            debug_rect.x + 10,
            debug_rect.y + 10 + debug_i * 20,
            debug_surface->w,
            debug_surface->h
        };
        SDL_RenderCopy(renderer, debug_texture, NULL, &debug_dst);
        SDL_FreeSurface(debug_surface);
        SDL_DestroyTexture(debug_texture);
    }
}

extern int debug_terminal_mode;

void execute_debug_command(const char* cmd, Player* player) {
    if (strncmp(cmd, "tp ", 3) == 0) {
        int debug_x, debug_y;
        if (sscanf(cmd + 3, "%d %d", &debug_x, &debug_y) == 2) {
            player->pos_x = debug_x * TILE_SIZE;
            player->pos_y = debug_y * TILE_SIZE;
            debug_terminal_log("    Teleportation a (%d %d)", debug_x, debug_y);
        } else {
            debug_terminal_log("    Syntaxe invalide : tp x y");
        }

    } else if (strcmp(cmd, "clear") == 0) {
        debug_clear(); // if you use a debug buffer
        debug_terminal_log(" ");

    } else if (strcmp(cmd, "help") == 0) {
        debug_terminal_log("Commandes disponibles :");
        debug_terminal_log("    tp x y         : téléporte le joueur en tile");
        debug_terminal_log("    chunks         : affiche le nombre de chunks actifs");
        debug_terminal_log("    clear          : nettoie le terminal debug");
        debug_terminal_log("    help           : affiche cette aide");

    } else if (strncmp(cmd, "debug ", 6) == 0) {
        if (strcmp(cmd + 6, "on") == 0) {
            debug_terminal_mode = 1;
            debug_terminal_log("    debug on");
        } else if (strcmp(cmd + 6, "off") == 0) {
            debug_terminal_mode = 0;
            debug_terminal_log("    debug off");
        } else {
            debug_terminal_log("    Syntaxe invalide : debug on/off");
        }
    }
    else {
        debug_terminal_log("    Commande inconnue");
    }

}

