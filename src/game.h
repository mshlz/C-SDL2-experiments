/** 
 * game.h
 * Mateus Holzschuh @ 2021 
 */

#ifndef GAME_H
#define GAME_H

#include "engine.h"

void start(engine_t *engine);
void update(engine_t *engine);
void render(engine_t *engine);
void on_event(SDL_Event *event);

#endif