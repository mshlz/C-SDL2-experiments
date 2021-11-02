/** 
 * snake.c
 * Mateus Holzschuh @ 2021 
 */

#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include "../shared/utils.h"

game_t *game = NULL;

/**
 * Run once when engine startup
 */
void start(engine_t *engine)
{
    game = malloc(sizeof(game_t));
    game->snake = malloc(sizeof(snake_t));

    // set default cell size
    game->rect.w = TILE_SIZE;
    game->rect.h = TILE_SIZE;

    // start running to the right
    game->snake->direction = DIR_RIGHT;
    game->next_direction = DIR_RIGHT;

    // create head and set in the center
    game->snake->head = malloc(sizeof(snake_cell_t));
    game->snake->head->position.x = COLS / 2;
    game->snake->head->position.y = ROWS / 2;
    game->snake->head->next = NULL;
    game->snake->head->prev = NULL;

    // as snake size is 1, the tail is the head
    game->snake->tail = game->snake->head;
    game->snake->size = 1;

    spawn_food();
}

/**
 * Run every frame
 */
void update(engine_t *engine)
{
    if (is_completed())
    {
        char buffer[100];
        snprintf(buffer, 100, "Congratulations!\nYou have completed the game.\nSNAKE SIZE: %d", game->snake->size);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You win!", buffer, engine->window);
        engine->isRunning = false;
    }
    else if (check_snake_collision(game->snake->head->position))
    {
        char buffer[100];
        snprintf(buffer, 100, "The snake collided with itself.\nYou lose!\nSNAKE SIZE: %d", game->snake->size);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "You lose!", buffer, engine->window);
        engine->isRunning = false;
    }
    else if (game->snake->head->position.x == game->next_food_position.x && game->snake->head->position.y == game->next_food_position.y)
    {
        enqueue_food(game->next_food_position);
        spawn_food();
    }

    if (should_append_food(game->snake->tail->position))
    {
        grow_snake();
    }

    game->snake->direction = game->next_direction;
    move_snake();
}

/**
 * Run every frame after the engine->update
 */
void render(engine_t *engine)
{
    // clear the screen
    SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 0);
    SDL_RenderClear(engine->renderer);

    render_food(engine);
    render_snake(engine);
    render_food_queue(engine);

    // execute render
    SDL_RenderPresent(engine->renderer);
}

/**
 * Run when an event is triggered (keyboard, mouse, etc)
 * Receives the SDL_Event from SDL_PoolEvent
 */
void on_event(engine_t *engine, SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
        case SDLK_UP:
            if (game->snake->direction != DIR_DOWN)
                game->next_direction = DIR_UP;
            break;
        case SDLK_DOWN:
            if (game->snake->direction != DIR_UP)
                game->next_direction = DIR_DOWN;
            break;
        case SDLK_LEFT:
            if (game->snake->direction != DIR_RIGHT)
                game->next_direction = DIR_LEFT;
            break;
        case SDLK_RIGHT:
            if (game->snake->direction != DIR_LEFT)
                game->next_direction = DIR_RIGHT;
            break;
        }
    }

    // if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
    // {
    //     if (event->key.keysym.mod & KMOD_LSHIFT)
    //         game->current_delay = game->shift_delay;
    //     else {
    //         game->current_delay = game->delay;
    //     }
    // }
}

// - GAME FUNCTIONS -------------------------
int check_snake_collision(SDL_Point position)
{
    snake_cell_t *cell = game->snake->tail;
    while (cell->prev != NULL)
    {
        if (cell->position.x == position.x && cell->position.y == position.y)
        {
            return true;
        }
        cell = cell->prev;
    }
    return false;
}

void spawn_food()
{
    do
    {
        game->next_food_position.x = rand() % COLS;
        game->next_food_position.y = rand() % ROWS;
    } while (check_snake_collision(game->next_food_position));
}

void enqueue_food(SDL_Point position)
{
    food_node_t *food = malloc(sizeof(food_node_t));
    food->position.x = position.x;
    food->position.y = position.y;
    food->next = NULL;

    if (game->food_queue == NULL)
    {
        game->food_queue = food;
    }
    else
    {
        food_node_t *last = game->food_queue;
        while (last->next != NULL)
            last = last->next;
        last->next = food;
    }
}

void dequeue_food()
{
    if (game->food_queue != NULL)
    {
        food_node_t *ref = game->food_queue;
        game->food_queue = game->food_queue->next;
        free(ref);
    }
}

void grow_snake()
{
    snake_cell_t *cell = malloc(sizeof(snake_cell_t));
    cell->position.x = game->food_queue->position.x;
    cell->position.y = game->food_queue->position.y;

    cell->next = NULL;
    cell->prev = game->snake->tail;

    game->snake->tail->next = cell;
    game->snake->tail = cell;
    game->snake->size++;

    dequeue_food();
}

int should_append_food(SDL_Point position)
{
    return !(game->food_queue == NULL || game->food_queue->position.x != position.x || game->food_queue->position.y != position.y);
}

int is_completed()
{
    return (game->snake->size == ROWS * COLS);
}

void move_snake()
{
    snake_cell_t *cell = game->snake->tail;
    while (cell->prev != NULL)
    {
        cell->position.x = cell->prev->position.x;
        cell->position.y = cell->prev->position.y;
        cell = cell->prev;
    }

    switch (game->snake->direction)
    {
    case DIR_UP:
        if (game->snake->head->position.y - 1 > -1)
            game->snake->head->position.y--;
        else
            game->snake->head->position.y = ROWS - 1;
        break;
    case DIR_RIGHT:
        if (game->snake->head->position.x + 1 < COLS)
            game->snake->head->position.x++;
        else
            game->snake->head->position.x = 0;
        break;
    case DIR_DOWN:
        if (game->snake->head->position.y + 1 < ROWS)
            game->snake->head->position.y++;
        else
            game->snake->head->position.y = 0;
        break;
    case DIR_LEFT:
        if (game->snake->head->position.x - 1 > -1)
            game->snake->head->position.x--;
        else
            game->snake->head->position.x = COLS - 1;
        break;
    }
}

// - RENDER FUNCTIONS -----------------------
void render_snake(engine_t *engine)
{
    SDL_SetRenderDrawColor(engine->renderer, 255, 255, 255, 0);

    snake_cell_t *p = game->snake->tail;
    while (p->prev != NULL)
    {
        game->rect.x = p->position.x * TILE_SIZE;
        game->rect.y = p->position.y * TILE_SIZE;
        p = p->prev;
        SDL_RenderFillRect(engine->renderer, &game->rect);
    }

    SDL_SetRenderDrawColor(engine->renderer, 255, 0, 0, 0);
    game->rect.x = p->position.x * TILE_SIZE;
    game->rect.y = p->position.y * TILE_SIZE;
    SDL_RenderFillRect(engine->renderer, &game->rect);
}

void render_food(engine_t *engine)
{
    SDL_SetRenderDrawColor(engine->renderer, 0, 255, 0, 0);

    game->rect.x = game->next_food_position.x * TILE_SIZE;
    game->rect.y = game->next_food_position.y * TILE_SIZE;
    SDL_RenderFillRect(engine->renderer, &game->rect);
}

void render_food_queue(engine_t *engine)
{
    SDL_SetRenderDrawColor(engine->renderer, 255, 255, 0, 0);

    food_node_t *node = game->food_queue;
    while (node != NULL)
    {
        game->rect.x = node->position.x * TILE_SIZE;
        game->rect.y = node->position.y * TILE_SIZE;
        node = node->next;
        SDL_RenderFillRect(engine->renderer, &game->rect);
    }
}
