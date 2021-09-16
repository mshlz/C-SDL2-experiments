/** 
 * entrypoint
 * Mateus Holzschuh @ 2021 
 */
#include <SDL2/SDL.h>

#include "game.h"
#include "engine.h"

#define TITLE "Game"
#define WIDTH 600
#define HEIGHT 600

int main(int argc, char **argv)
{
    engine_t *engine = create_engine(
        TITLE,              // window title
        WIDTH,              // window width
        HEIGHT,             // window height
        SDL_WINDOW_TOOLTIP, // window flags

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
