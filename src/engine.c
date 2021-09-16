/** 
 * engine.c
 * Mateus Holzschuh @ 2021 
 */

#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "utils.h"

engine_t *create_engine_without_handlers(char *title, int width, int height, Uint32 SDL_Window_flags)
{
    engine_t *engine = malloc(sizeof(engine_t));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("[video] init failure: %s", SDL_GetError());
        return NULL;
    }

    engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_Window_flags);
    if (engine->window == NULL)
    {
        printf("[window] creation failure: %s", SDL_GetError());
        return NULL;
    }

    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);
    if (engine->renderer == NULL)
    {
        printf("[renderer] creation failure: %s", SDL_GetError());
        return NULL;
    }

    engine->isRunning = true;
    engine->delay = DEFAULT_ENGINE_DELAY;

    return engine;
}

engine_t *create_engine(
    char *title,
    int width,
    int height,
    Uint32 SDL_Window_flags,
    void (*start)(engine_t *engine),
    void (*update)(engine_t *engine),
    void (*render)(engine_t *engine),
    void (*on_event)(SDL_Event *event))
{
    engine_t *engine = create_engine_without_handlers(title, width, height, SDL_Window_flags);

    if (engine == NULL)
    {
        return NULL;
    }

    engine->start = start;
    engine->update = update;
    engine->render = render;
    engine->on_event = on_event;

    return engine;
}

int run_engine(engine_t *engine)
{
    engine->start(engine);

    SDL_Event event;
    while (engine->isRunning)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            engine->on_event(&event);

            if (event.type == SDL_QUIT)
            {
                engine->isRunning = false;
            }
        }

        engine->update(engine);

        engine->render(engine);

        SDL_Delay(engine->delay);
    }

    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
    free(engine);

    return EXIT_SUCCESS;
}