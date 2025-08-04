#include "../../include/global.h"

bool debug_mode = false;

#define DEBUG_BUFFER_LINES 8

static char debug_lines[DEBUG_BUFFER_LINES][128]; // 5 lignes max
static int debug_line_count = 0;

void debug_clear() {
    debug_line_count = 0;
}

void debug_terminal_log(const char* fmt, ...) {
    if (debug_line_count >= DEBUG_BUFFER_LINES) {
        // décale les lignes (scroll)
        for (int i = 1; i < DEBUG_BUFFER_LINES; i++) {
            strcpy(debug_lines[i - 1], debug_lines[i]);
        }
        debug_line_count = DEBUG_BUFFER_LINES - 1;
    }

    va_list args;
    va_start(args, fmt);
    vsnprintf(debug_lines[debug_line_count++], 128, fmt, args);
    va_end(args);
}

void debug_render_in_rect(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect rect) {
    SDL_Color white = {255, 255, 255, 255};
    for (int i = 0; i < debug_line_count; i++) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, debug_lines[i], white);  // UTF-8
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = {
            rect.x + 10,
            rect.y + 10 + i * 20,
            surface->w,
            surface->h
        };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

extern int debug_terminal_mode;

void execute_debug_command(const char* cmd, Player* player) {
    if (strncmp(cmd, "tp ", 3) == 0) {
        int x, y;
        if (sscanf(cmd + 3, "%d %d", &x, &y) == 2) {
            player->pos_x = x * TILE_SIZE;
            player->pos_y = y * TILE_SIZE;
            debug_terminal_log("", x, y);
        } else {
            debug_terminal_log("    Syntaxe invalide : tp x y");
        }

    } else if (strcmp(cmd, "clear") == 0) {
        debug_clear(); // si tu utilises un buffer debug
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

