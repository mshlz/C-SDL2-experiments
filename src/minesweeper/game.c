/** 
 * Minesweeper
 * game.c
 * Mateus Holzschuh @ 2021 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "../shared/utils.h"

int pixel_to_idx(int windowX, int windowY)
{
    return (windowX / TILE_SIZE) + ((windowY / TILE_SIZE) * game.width);
}

int coord_to_idx(int x, int y)
{
    if (x < 0 || x >= game.width || y < 0 || y >= game.height)
        return -1;
    return x + y * game.width;
}

void idx_to_coord(int idx, int *x, int *y)
{
    int a = idx / game.width;
    *x = (idx - (game.width * a));
    *y = a;
}

int *get_adjacents(int idx, int *array_size)
{
    int x, y, z;
    idx_to_coord(idx, &x, &y);

    int *arr = malloc(sizeof(int) * 8);
    int size = 0;

    int adjX[] = {x-1,   x, x+1, x-1, x+1, x-1,   x, x+1};
    int adjY[] = {y-1, y-1, y-1,   y,   y, y+1, y+1, y+1};

    for (int i=0; i<8; i++) {
        z = coord_to_idx(adjX[i], adjY[i]);
        if (z != -1)
            arr[size++] = z;
    }

    *array_size = size;
    return arr;
}

int count_adjacent_bombs(int idx)
{
    int bombs_count = 0, adjacents_count;
    int *adjacents = get_adjacents(idx, &adjacents_count);

    for (int i = 0; i < adjacents_count; i++)
    {
        if (game.board[adjacents[i]] == TILE_BOMB)
            bombs_count++;
    }

    return bombs_count;
}

void reveal_board()
{
    for (int i = 0; i < game.size; i++)
    {
        game.screen_board[i] = game.board[i];
    }
}

void create_board(int start_idx)
{
    for (int i = 0; i < game.bomb_count; i++)
    {
        int idx = coord_to_idx(rand() % game.width, rand() % game.height);
        if (idx == start_idx)
            continue;
        game.board[idx] = TILE_BOMB;
    }

    for (int i = 0; i < game.size; i++)
    {
        if (game.board[i] != TILE_BOMB)
        {
            game.board[i] = count_adjacent_bombs(i);
        }
    }
}

void expand_empty(int idx, int borderReached)
{
    game.screen_board[idx] = game.board[idx];

    int adjacents_count;
    int *adjacents = get_adjacents(idx, &adjacents_count);

    int z;
    for (int i = 0; i < adjacents_count; i++)
    {
        z = adjacents[i];
        if (game.screen_board[z] == TILE_UNTOUCHED)
        {
            if (game.board[z] == TILE_0)
                expand_empty(z, false);
            else if (borderReached == false && game.board[z] >= TILE_1 && game.board[z] <= TILE_8)
                expand_empty(z, true);
        }
    }
}

int won()
{
    int won = true;
    for (int i = 0; i < game.size; i++)
    {
        if (game.board[i] != TILE_BOMB && (game.screen_board[i] >= TILE_UNTOUCHED && game.screen_board[i] <= TILE_DOUBT))
        {
            won = false;
        }
    }
    return !game.failed && won;
}

// rect to slice & render tiles
SDL_Rect from, to;

/**
 * Run once when engine startup
 */
void start(engine_t *engine)
{
    srand(time(0));

    IMG_Init(IMG_INIT_PNG);
    game.figures_texture = IMG_LoadTexture(engine->renderer, "./images/minesweeper.png");

    from.w = from.h = to.w = to.h = TILE_SIZE;
    from.y = 0;

    game.board = malloc(sizeof(int) * game.size);
    game.screen_board = malloc(sizeof(int) * game.size);

    for (int i = 0; i < game.size; i++)
    {
        game.screen_board[i] = TILE_UNTOUCHED;
    }

    game.has_started = false;
    game.failed = false;
}

/**
 * Run every frame
 */
void update(engine_t *engine)
{
}

/**
 * Run every frame after the engine->update
 */
void render(engine_t *engine)
{
    // clear the screen
    SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 0);
    SDL_RenderClear(engine->renderer);

    for (int i = 0; i < game.size; i++)
    {
        from.x = game.screen_board[i] * TILE_SIZE;

        int x, y;
        idx_to_coord(i, &x, &y);

        to.x = x * TILE_SIZE;
        to.y = y * TILE_SIZE;

        SDL_RenderCopy(engine->renderer, game.figures_texture, &from, &to);
    }

    // execute render
    SDL_RenderPresent(engine->renderer);
}

/**
 * Run when an event is triggered (keyboard, mouse, etc)
 * Receives the SDL_Event from SDL_PoolEvent
 */
void on_event(engine_t *engine, SDL_Event *event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        int idx = pixel_to_idx(event->button.x, event->button.y);

        // If right button is pressed
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            if (game.screen_board[idx] == TILE_UNTOUCHED || game.screen_board[idx] == TILE_FLAG)
                game.screen_board[idx]++;
            else if (game.screen_board[idx] == TILE_DOUBT)
                game.screen_board[idx] = TILE_UNTOUCHED;
        }

        // If left button is pressed
        else if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (game.screen_board[idx] >= TILE_UNTOUCHED && game.screen_board[idx] <= TILE_DOUBT)
            {
                game.last_idx = idx;
                game.last_idx_flag = game.screen_board[idx];
                game.screen_board[idx] = TILE_PRESSED;
            }
            else
            {
                game.last_idx = -1;
            }
        }
    }

    else if (event->type == SDL_MOUSEBUTTONUP)
    {
        // If left button is released
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            int idx = pixel_to_idx(event->button.x, event->button.y);

            if (game.last_idx != -1)
            {
                if (game.last_idx != idx)
                {
                    game.screen_board[game.last_idx] = game.last_idx_flag;
                }
                else
                {
                    if (!game.has_started)
                    {
                        create_board(game.last_idx);
                        game.has_started = true;
                    }

                    game.screen_board[game.last_idx] = game.board[game.last_idx];

                    if (game.board[game.last_idx] == TILE_BOMB)
                    {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "You lose!", "Ops! You triggered the bomb!", engine->window);
                        game.failed = true;
                        reveal_board();
                    }

                    else if (game.board[game.last_idx] == TILE_0)
                    {
                        expand_empty(game.last_idx, false);
                    }
                }
            }
        }

        if (won())
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Congratulations!", "You won the game!", engine->window);
            reveal_board();
        }
    }
}