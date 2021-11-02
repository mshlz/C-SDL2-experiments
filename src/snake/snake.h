/** 
 * snake.h
 * Mateus Holzschuh @ 2021 
 */

#ifndef GAME_H
#define GAME_H

#include "../shared/engine.h"

/**
 * Settings
 */
#define TILE_SIZE 20
#define ROWS 40
#define COLS 40

#define TITLE "Snake"
#define WIDTH (COLS * TILE_SIZE)
#define HEIGHT (ROWS * TILE_SIZE)

/**
 * Directions
 */
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

/**
 * Snake
 */
typedef struct snake_cell
{
    SDL_Point position;
    struct snake_cell *prev;
    struct snake_cell *next;
} snake_cell_t;

typedef struct snake
{
    snake_cell_t *head;
    snake_cell_t *tail;
    int8_t direction;
    int size;
} snake_t;

typedef struct food_node
{
    SDL_Point position;
    struct food_node *next;
} food_node_t;

/**
 * Game state
 */

typedef struct game
{
    snake_t *snake;
    int8_t next_direction;

    SDL_Point next_food_position;
    food_node_t *food_queue;

    SDL_Rect rect;
} game_t;


int check_snake_collision(SDL_Point position);
void spawn_food();
void enqueue_food(SDL_Point position);
void dequeue_food();
void grow_snake();
int should_append_food(SDL_Point position);
int is_completed();
void move_snake();


void render_snake(engine_t *engine);
void render_food(engine_t *engine);
void render_food_queue(engine_t *engine);

/**
 * Engine methods
 */
void start(engine_t *engine);
void update(engine_t *engine);
void render(engine_t *engine);
void on_event(engine_t *engine, SDL_Event *event);

#endif