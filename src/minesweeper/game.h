/** 
 * Minesweeper
 * Mateus Holzschuh @ 2021 
 */

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_image.h>
#include "../shared/engine.h"

/**
 * Default Settings (it try to load from config.txt first)
 */
#define TILE_SIZE 64
#define COLS 16
#define ROWS 10
#define BOMB_RATIO 0.2

#define TITLE "Minesweeper - @mshlz"

enum Tiles {
    TILE_0 = 0,
    TILE_1,
    TILE_2,
    TILE_3,
    TILE_4,
    TILE_5,
    TILE_6,
    TILE_7,
    TILE_8,
    TILE_UNTOUCHED,
    TILE_FLAG,
    TILE_DOUBT,
    TILE_PRESSED,
    TILE_BOMB,
};

/**
 * Game state
 */
struct game_t
{
    int *screen_board;
    int *board;
    int width;
    int height;
    int size;
    int bomb_count;

    int has_started;
    int failed;

    int last_idx;
    int last_idx_flag;

    SDL_Texture *figures_texture;
} game;


/**
 * Engine methods
 */
void start(engine_t *engine);
void update(engine_t *engine);
void render(engine_t *engine);
void on_event(engine_t *engine, SDL_Event *event);

#endif