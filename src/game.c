/** 
 * game.c
 * Mateus Holzschuh @ 2021 
 */

#include <stdio.h>
#include "game.h"

/**
 * Run once when engine startup
 */
void start(engine_t *engine)
{
    printf("::start\n");
}

/**
 * Run every frame
 */
void update(engine_t *engine)
{
    printf("::update\n");
}

/**
 * Run every frame after the engine->update
 */
void render(engine_t *engine)
{
    printf("::render\n");
}

/**
 * Run when an event is triggered (keyboard, mouse, etc)
 * Receives the SDL_Event from SDL_PoolEvent
 */
void on_event(SDL_Event *event)
{
}
