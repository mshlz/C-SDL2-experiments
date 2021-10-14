/** 
 * engine.h
 * Mateus Holzschuh @ 2021 
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>

#define DEFAULT_ENGINE_DELAY 17

typedef struct engine_t
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    int8_t isRunning;
    int32_t delay;

    void (*start)(struct engine_t *engine);
    void (*update)(struct engine_t *engine);
    void (*render)(struct engine_t *engine);
    void (*on_event)(struct engine_t *engine, SDL_Event *event);
} engine_t;

engine_t *create_engine_without_handlers(char *title, int width, int height, Uint32 SDL_flags);

engine_t *create_engine(
    char *title,
    int width,
    int height,
    Uint32 SDL_flags,
    void (*start)(engine_t *engine),
    void (*update)(engine_t *engine),
    void (*render)(engine_t *engine),
    void (*on_event)(struct engine_t *engine, SDL_Event *event));

int run_engine(engine_t *engine);

#endif