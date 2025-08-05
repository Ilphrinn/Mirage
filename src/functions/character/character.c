#include "../../include/global.h"

static bool is_walkable(int64_t pos_x, int64_t pos_y, SDL_Rect hitbox) {
    int left   = (int)(pos_x + hitbox.x);
    int right  = (int)(pos_x + hitbox.x + hitbox.w - 1);
    int top    = (int)(pos_y + hitbox.y);
    int bottom = (int)(pos_y + hitbox.y + hitbox.h - 1);

    TileID tl = get_tile_at(left / TILE_SIZE, top / TILE_SIZE);
    TileID tr = get_tile_at(right / TILE_SIZE, top / TILE_SIZE);
    TileID bl = get_tile_at(left / TILE_SIZE, bottom / TILE_SIZE);
    TileID br = get_tile_at(right / TILE_SIZE, bottom / TILE_SIZE);

    return (tl != TILE_WALL && tr != TILE_WALL && bl != TILE_WALL && br != TILE_WALL &&
            tl != TILE_MOUNTAIN && tr != TILE_MOUNTAIN && bl != TILE_MOUNTAIN && br != TILE_MOUNTAIN);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Player create_player(int x, int y, int size, int speed) {
    Player p = {
    .pos_x = x,
    .pos_y = y,
    .rect = {x, y, size, size},
    .hitbox = {size/8, size/8, size - (size/4), size - (size/4)},
    .speed = speed,
    .color = {200, 0, 0, 255}
    };
    return p;
}

void move_player(Player* player, const Uint8* keystates) {
    float speed = player->speed;

    // Compute the center to determine the tile under the player
    int center_x = (int)(player->pos_x + player->rect.w / 2);
    int center_y = (int)(player->pos_y + player->rect.h / 2);
    int tile_x = center_x / TILE_SIZE;
    int tile_y = center_y / TILE_SIZE;

    TileID current_tile = get_tile_at(tile_x, tile_y);
    if (current_tile == TILE_WATER) {
        speed *= 0.8f;
    }

    // Next positions
    int64_t next_x = player->pos_x;
    int64_t next_y = player->pos_y;

    bool move_x = false, move_y = false;
    if (keystates[SDL_SCANCODE_LEFT])  { next_x -= speed; move_x = true; }
    if (keystates[SDL_SCANCODE_RIGHT]) { next_x += speed; move_x = true; }
    if (keystates[SDL_SCANCODE_UP])    { next_y -= speed; move_y = true; }
    if (keystates[SDL_SCANCODE_DOWN])  { next_y += speed; move_y = true; }

    // Case 1: full movement
    if (is_walkable(next_x, next_y, player->hitbox)) {
        player->pos_x = next_x;
        player->pos_y = next_y;
    }
    // Case 2: only X
    else if (move_x && is_walkable(next_x, player->pos_y, player->hitbox)) {
        player->pos_x = next_x;
    }
    // Case 3: only Y
    else if (move_y && is_walkable(player->pos_x, next_y, player->hitbox)) {
        player->pos_y = next_y;
    }

    if (player->pos_x < 0) player->pos_x = 0;
    if (player->pos_y < 0) player->pos_y = 0;

    if (player->pos_x + player->rect.w > MAP_PIXEL_WIDTH)
        player->pos_x = MAP_PIXEL_WIDTH - player->rect.w;

    if (player->pos_y + player->rect.h > MAP_PIXEL_HEIGHT)
        player->pos_y = MAP_PIXEL_HEIGHT - player->rect.h;


    // Update the rectangle
    player->rect.x = (int)player->pos_x;
    player->rect.y = (int)player->pos_y;
}

void update_camera(SDL_Rect* camera, const Player* player) {
    int64_t cam_x = player->pos_x + player->rect.w / 2 - camera->w / 2;
    int64_t cam_y = player->pos_y + player->rect.h / 2 - camera->h / 2;

    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;
    if (cam_x + camera->w > MAP_PIXEL_WIDTH) cam_x = MAP_PIXEL_WIDTH - camera->w;
    if (cam_y + camera->h > MAP_PIXEL_HEIGHT) cam_y = MAP_PIXEL_HEIGHT - camera->h;

    camera->x = (int)cam_x;
    camera->y = (int)cam_y;
}

void render_player(SDL_Renderer* renderer, const Player* player, const SDL_Rect* camera) {
    SDL_Rect dst = {
        (int)(player->pos_x - camera->x),
        (int)(player->pos_y - camera->y),
        player->rect.w,
        player->rect.h
    };

    SDL_SetRenderDrawColor(renderer, player->color.r, player->color.g, player->color.b, player->color.a);
    SDL_RenderFillRect(renderer, &dst);
}