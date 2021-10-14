/** 
 * entrypoint
 * Mateus Holzschuh @ 2021 
 */
#include <SDL2/SDL.h>
#include <stdio.h>

#include "../shared/engine.h"
#include "game.h"

void read_prefs()
{
    // DEFAULT
    game.width = COLS;
    game.height = ROWS;
    game.size = game.width * game.height;
    game.bomb_count = game.size * BOMB_RATIO;

    FILE *settings_file = fopen("./config.txt", "r");
    if (settings_file == NULL)
    {
        return;
    }
    int value = -1;
    fscanf(settings_file, "COLS=%d\n", &value);
    if (value != -1)
        game.width = value;

    value = -1;
    fscanf(settings_file, "ROWS=%d\n", &value);
    if (value != -1)
        game.height = value;

    value = -1;
    fscanf(settings_file, "MAX_BOMB_COUNT=%d\n", &value);
    if (value != -1)
        game.bomb_count = value;

    game.size = game.width * game.height;
}

int main(int argc, char **argv)
{
    read_prefs();

    engine_t *engine = create_engine(
        TITLE,                   // window title
        game.width * TILE_SIZE,  // window width
        game.height * TILE_SIZE, // window height
        SDL_WINDOW_TOOLTIP,      // window flags

        start,   // engine::start - run once at startup
        update,  // engine::update - run every frame
        render,  // engine::render - run every frame after the engine::update call
        on_event // engine::on_event - run after any event from SDL_PoolEvent (keyboard, mouse, etc)
    );

    if (engine == NULL)
    {
        return -1;
    }

    return run_engine(engine);
}
