#ifndef BRICK_H
#define BRICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <time.h>

#include "../common/common.h"

#define BORDER_SIZE 3
#define TEMP_FIELD_Y FIELD_Y + BORDER_SIZE
#define TEMP_FIELD_X FIELD_X + BORDER_SIZE + BORDER_SIZE
#define FIGURE_SIZE 4
#define FIGURE_COUNT 7
#define LEVEL_THRESHOLD 600
#define START_SPEED 10
#define SPEED_INCREMENT 1
#define MAX_LEVEL 10

typedef struct {
  int pos_y, pos_x;
  int **current;
} Figure_t;

typedef enum {
  START,
  PAUSE,
  SPAWN,
  GAME,
  ATTACH,
  GAMEOVER,
  EXITSTATE,
} GameState_t;

typedef struct {
  GameInfo_t *data;
  GameState_t *state;
  Figure_t *figure;
  int **temp_field;
  struct timespec *last_move_time;
} GameParams_t;

typedef void (*funcPointer)(GameParams_t *params);
GameParams_t *initialize_params();
GameParams_t *params_holder();
void start_game(GameParams_t *params);
void pause_game(GameParams_t *params);
void spawn_figure(GameParams_t *params);
void restart_game(GameParams_t *params);
void move_down(GameParams_t *params);
void move_right(GameParams_t *params);
void move_left(GameParams_t *params);
void rotate_figure(GameParams_t *params);
void make_step(GameParams_t *params);
void attaching(GameParams_t *params);
void terminate(GameParams_t *params);
// funcPointer fsm(GameState_t state,UserAction_t action);
// int (*random_figure_generator())[FIGURE_SIZE];

bool time_for_step(struct timespec *last_move_time,
                   GameInfo_t *current_state_ptr);
bool figure_collided(int **field, int **figure, int pos_y, int pos_x);
void remove_figure(int **field, Figure_t *figure);
void paste_figure(int **field, Figure_t *figure);
int remove_line(int **field);
void score_counting(int *score, int removed_lines);
void speed_level_adjusting(int *speed, int *level, int score);

void clear_field(int **field);
void copy_from_tepm_field(int **temp_field, int **field);
void copy_from_static_array(int (*source)[FIGURE_SIZE], int **destination,
                            int rows, int cols);
void copy_array(int **source, int **destination, int rows, int cols);
int **allocate_array(int nRows, int nCols);
void deallocate_array(int **A, int nRows);
int ***allocate_3D_array(int layers, int rows, int cols);

#ifdef __cplusplus
}
#endif

#endif