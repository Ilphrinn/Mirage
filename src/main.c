#include "include/global.h"

#define STARTING_X 0
#define STARTING_Y 0

#define PLAYER_SIZE 32
#define PLAYER_SPEED 5

// --- Variables pour le terminal debug ---
int debug_terminal_mode = 0; // 0 = affichage, 1 = saisie de commande
char debug_input[128] = "";
int debug_input_len = 0;

int main(int argc, char *argv[]) {

//////////////////////// CHARGEMENT ////////////////////////

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return 1;
    }

    // Récupération de la taille de l'écran
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        SDL_Log("Erreur SDL_GetDesktopDisplayMode: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int screen_width = dm.w;
    int screen_height = dm.h;

    // Création de la fenêtre SDL
    SDL_Window *window = SDL_CreateWindow(
        "Projet SDL - Modulaire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

    if (!window) {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialisation de la police
    if (TTF_Init() < 0) {
        SDL_Log("Erreur TTF_Init: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 16);
    if (!font) {
        SDL_Log("Erreur TTF_OpenFont: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialisation de la carte
    map_init();

    // Création du joueur et de la caméra
    Player player = create_player(STARTING_X, STARTING_Y, PLAYER_SIZE, PLAYER_SPEED);
    SDL_Rect camera = {STARTING_X, STARTING_Y, screen_width, screen_height};

if (debug_mode) {
    debug_clear();          // Réinitialisation du buffer debug
    SDL_StartTextInput();   // Autorise la saisie de texte pour le terminal
}

    // Variables de FPS
    int frame_count = 0;
    Uint32 last_time = SDL_GetTicks();
    int fps = 0;
    SDL_Event event;
    bool running = true;

//////////////////////////////////////////////////////////////////



///////////////////////////// BOUCLE PRINCIPALE /////////////////////////////

    while (running) {

        // Gestion des événements SDL
        while (SDL_PollEvent(&event)) {

//// --- DEBUG MODE --- ////
if (event.type == SDL_KEYDOWN &&
    event.key.keysym.sym == SDLK_p &&
    (event.key.keysym.mod & KMOD_ALT) &&
    (event.key.keysym.mod & KMOD_SHIFT)) {

    debug_mode = !debug_mode;
    debug_terminal_mode = 0;
    debug_input_len = 0;
    debug_input[0] = '\0';

    debug_terminal_log("[DEBUG] Mode %s", debug_mode ? "activé" : "désactivé");
}

if (debug_mode) {
            // Bascule terminal : TAB → mode affichage <-> saisie
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
                debug_terminal_mode = !debug_terminal_mode;
                debug_input_len = 0;
                debug_input[0] = '\0';

            // En mode terminal : saisie de texte
            } else if (debug_terminal_mode && event.type == SDL_TEXTINPUT) {
                if (debug_input_len < sizeof(debug_input) - 1) {
                    strcat(debug_input, event.text.text);
                    debug_input_len++;
                }

            // En mode terminal : gestion retour et validation
            } else if (debug_terminal_mode && event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && debug_input_len > 0) {
                    debug_input[--debug_input_len] = '\0';
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    debug_terminal_log("> %s", debug_input);
                    execute_debug_command(debug_input, &player); // à implémenter selon tes besoins
                    debug_input_len = 0;
                    debug_input[0] = '\0';
                }
            }
}
//// ---   --- ////

            // Quitter le jeu
            if (event.type == SDL_QUIT || 
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }

        // Mise à jour des entrées clavier
        const Uint8 *keystates = SDL_GetKeyboardState(NULL);
        move_player(&player, keystates);
        update_camera(&camera, &player);

        // Chargement dynamique des chunks
        int cam_tile_x = camera.x / TILE_SIZE;
        int cam_tile_y = camera.y / TILE_SIZE;
        Load_Chunks(cam_tile_x, cam_tile_y);

        // Rendu principal
        render_floor(renderer, camera);
        render_player(renderer, &player, &camera);

        // Calcul des FPS
        frame_count++;
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_time >= 1000) {
            fps = frame_count;
            frame_count = 0;
            last_time = current_time;
        }

if (debug_mode) {
    render_fps(renderer, font, fps);
    render_minimap(renderer, player.pos_x, player.pos_y);
    render_debug_terminal(renderer, font, screen_width, screen_height, camera.x, camera.y);
}

        SDL_RenderPresent(renderer);
    }

//////////////////////////////////////////////////////////////////



////////////////////////// DECHARGEMENT //////////////////////////

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

//////////////////////////////////////////////////////////////////
}