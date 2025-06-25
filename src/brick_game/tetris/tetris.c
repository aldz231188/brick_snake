#include "tetris.h"

#include "controller.h"
#include "figures.h"
#include "fsm.h"
#include "time.h"

GameInfo_t updateCurrentState() {
  GameParams_t *params = params_holder();
  if (*params->state != EXITSTATE) {
    userInput((UserAction_t)-1, 0);
    if (*params->state == GAME &&
        time_for_step(params->last_move_time, params->data)) {
      make_step(params);
    }
    copy_from_tepm_field(params->temp_field, params->data->field);
  }
  return *params_holder()->data;
}

GameParams_t *params_holder() {
  static bool first = true;
  static GameParams_t params;
  if (first) {
    params = *initialize_params();
    first = false;
  }
  return &params;
}

GameParams_t *initialize_params() {
  static GameState_t fsm_state;
  static Figure_t figure = {0};
  static GameInfo_t current_state;
  static GameParams_t params;
  static int **temp_field;
  static struct timespec last_move_time;

  params.last_move_time = &last_move_time;
  params.data = &current_state;
  params.figure = &figure;
  params.state = &fsm_state;
  *params.state = START;
  params.data->speed = START_SPEED;
  params.data->level = 1;
  params.data->pause = 0;
  params.data->score = 0;
  read_highscore(&params.data->high_score);
  temp_field = allocate_array(TEMP_FIELD_Y, TEMP_FIELD_X);
  params.temp_field = temp_field;
  params.data->field = allocate_array(FIELD_Y, FIELD_X);
  params.data->next = allocate_array(FIGURE_SIZE, FIGURE_SIZE);
  params.figure->current = allocate_array(FIGURE_SIZE, FIGURE_SIZE);
  copy_from_static_array(random_figure_generator(), params.data->next,
                         FIGURE_SIZE, FIGURE_SIZE);
  clock_gettime(CLOCK_REALTIME, &last_move_time);
  return &params;
}

void start_game(GameParams_t *params) { *params->state = SPAWN; }

void pause_game(GameParams_t *params) {
  if (*params->state == GAME) {
    *params->state = PAUSE;
    params->data->pause = 1;
  } else if (*params->state == PAUSE) {
    *params->state = GAME;
    params->data->pause = 0;
  }
}

void restart_game(GameParams_t *params) {
  if (params->data->score > params->data->high_score) {
    write_highscore(params->data->score);
    params->data->high_score = params->data->score;
  }
  copy_from_static_array(random_figure_generator(), params->data->next,
                         FIGURE_SIZE, FIGURE_SIZE);
  *params->state = SPAWN;
  params->data->score = 0;
  params->data->level = 1;
  params->data->speed = START_SPEED;
  params->data->pause = 0;
  clear_field(params->temp_field);
  clock_gettime(CLOCK_REALTIME, params->last_move_time);
}

void terminate(GameParams_t *params) {
  *params->state = EXITSTATE;
  if (params->data->score > params->data->high_score) {
    write_highscore(params->data->score);
  }
  deallocate_array(params->data->field, FIELD_Y);
  deallocate_array(params->temp_field, TEMP_FIELD_Y);
  deallocate_array(params->data->next, FIGURE_SIZE);
  deallocate_array(params->figure->current, FIGURE_SIZE);
  params->data->pause = 3;
}

void move_down(GameParams_t *params) {
  if (!figure_collided(params->temp_field, params->figure->current,
                       params->figure->pos_y + 1, params->figure->pos_x)) {
    remove_figure(params->temp_field, params->figure);
    params->figure->pos_y++;
    paste_figure(params->temp_field, params->figure);
  }
}

void move_left(GameParams_t *params) {
  if (!figure_collided(params->temp_field, params->figure->current,
                       params->figure->pos_y, params->figure->pos_x - 1)) {
    remove_figure(params->temp_field, params->figure);
    params->figure->pos_x--;
    paste_figure(params->temp_field, params->figure);
  }
}

void move_right(GameParams_t *params) {
  if (!figure_collided(params->temp_field, params->figure->current,
                       params->figure->pos_y, params->figure->pos_x + 1)) {
    remove_figure(params->temp_field, params->figure);
    params->figure->pos_x++;
    paste_figure(params->temp_field, params->figure);
  }
}

void rotate_figure(GameParams_t *params) {
  remove_figure(params->temp_field, params->figure);
  int **temp = allocate_array(FIGURE_SIZE, FIGURE_SIZE);
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      temp[x][FIGURE_SIZE - 1 - y] = params->figure->current[y][x];
    }
  }
  if (!figure_collided(params->temp_field, temp, params->figure->pos_y,
                       params->figure->pos_x))
    copy_array(temp, params->figure->current, FIGURE_SIZE, FIGURE_SIZE);
  paste_figure(params->temp_field, params->figure);
  deallocate_array(temp, FIGURE_SIZE);
}

void spawn_figure(GameParams_t *params) {
  params->figure->pos_x = 3 + BORDER_SIZE - 1;
  params->figure->pos_y = 0;
  copy_array(params->data->next, params->figure->current, FIGURE_SIZE,
             FIGURE_SIZE);
  copy_from_static_array(random_figure_generator(), params->data->next,
                         FIGURE_SIZE, FIGURE_SIZE);
  *params->state = GAME;
}

void make_step(GameParams_t *params) {
  if (!figure_collided(params->temp_field, params->figure->current,
                       params->figure->pos_y + 1, params->figure->pos_x)) {
    move_down(params);
  } else if (params->figure->pos_y < 2) {
    *params->state = GAMEOVER;
    params->data->pause = 2;
  } else
    *params->state = ATTACH;
}

void attaching(GameParams_t *params) {
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (params->figure->current[y][x])
        params
            ->temp_field[y + params->figure->pos_y][x + params->figure->pos_x] =
            1;
    }
  }
  int removed_lines = remove_line(params->temp_field);
  score_counting(&params->data->score, removed_lines);
  speed_level_adjusting(&params->data->speed, &params->data->level,
                        params->data->score);
  *params->state = SPAWN;
}

bool time_for_step(struct timespec *last_move_time,
                   GameInfo_t *current_state_ptr) {
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  double elapsed_time =
      (current_time.tv_sec - (*last_move_time).tv_sec) +
      (current_time.tv_nsec - (*last_move_time).tv_nsec) / 1e9;
  double interval =
      current_state_ptr->speed /
      ((current_state_ptr->level * 0.5 + 0.4) * (current_state_ptr->speed));
  if (elapsed_time >= interval) {
    *last_move_time = current_time;
    return true;
  } else {
    return false;
  }
}

void speed_level_adjusting(int *speed, int *level, int score) {
  if (score / LEVEL_THRESHOLD + 1 > *level && *level <= MAX_LEVEL) {
    *speed -= SPEED_INCREMENT * score / LEVEL_THRESHOLD + 1;
    *level = score / LEVEL_THRESHOLD + 1;
    if (*level > 10) *level = 10;
  }
}

void score_counting(int *score, int removed_lines) {
  if (removed_lines == 1) {
    *score += 100;
  } else if (removed_lines == 2) {
    *score += 300;
  } else if (removed_lines == 3) {
    *score += 700;
  } else if (removed_lines > 3) {
    *score += 1500;
  }
}

bool figure_collided(int **field, int **figure, int pos_y, int pos_x) {
  bool collided = false;
  for (int y = 0; y < FIGURE_SIZE && !collided; y++) {
    for (int x = 0; x < FIGURE_SIZE && !collided; x++) {
      if (figure[y][x] &&
          (field[y + pos_y][x + pos_x] == 1 || y + pos_y == TEMP_FIELD_Y - 1 ||
           x + pos_x > FIELD_X + 1 || x + pos_x < BORDER_SIZE - 1))
        collided = true;
    }
  }
  return collided;
}

void remove_figure(int **field, Figure_t *figure) {
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (y + figure->pos_y < TEMP_FIELD_Y && figure->current[y][x])
        field[y + figure->pos_y][x + figure->pos_x] = 0;
    }
  }
}

void paste_figure(int **field, Figure_t *figure) {
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (y + figure->pos_y < TEMP_FIELD_Y)
        if (field[y + figure->pos_y][x + figure->pos_x] != 1)
          field[y + figure->pos_y][x + figure->pos_x] = figure->current[y][x];
    }
  }
}

int remove_line(int **field) {
  int removed_lines = 0;
  for (int y = TEMP_FIELD_Y - 1; y >= 0; y--) {
    int line_must_be_removed = 1;
    for (int x = BORDER_SIZE - 1;
         x < TEMP_FIELD_X - BORDER_SIZE - 1 && line_must_be_removed; x++) {
      if (field[y][x] == 0) line_must_be_removed = 0;
    }
    if (line_must_be_removed) {
      removed_lines++;
      for (int i = BORDER_SIZE - 1; i < TEMP_FIELD_X - BORDER_SIZE - 1; i++)
        field[y][i] = 0;
    } else {
      for (int i = BORDER_SIZE - 1; i < TEMP_FIELD_X - BORDER_SIZE - 1; i++)
        field[y + removed_lines][i] = field[y][i];
    }
  }
  return removed_lines;
}

void clear_field(int **field) {
  for (int i = 0; i < TEMP_FIELD_Y; i++) {
    for (int j = 0; j < TEMP_FIELD_X; j++) {
      field[i][j] = 0;
    }
  }
}

int **allocate_array(int rows, int nCols) {
  int **arr = (int **)calloc(rows, sizeof(int *));
  for (size_t rowIdx = 0; rowIdx < (size_t)rows; rowIdx++)
    arr[rowIdx] = (int *)calloc(nCols, sizeof(int));
  return arr;
}

void deallocate_array(int **arr, int rows) {
  if (arr != NULL) {
    for (int i = 0; i < rows; i++) {
      free(arr[i]);
    }
    free(arr);
  }
}

void copy_from_tepm_field(int **temp_field, int **field) {
  for (int y = 0; y < FIELD_Y; y++)
    for (int x = 0; x < FIELD_X; x++) {
      field[y][x] = temp_field[y + BORDER_SIZE - 1][x + BORDER_SIZE - 1];
    }
}

void copy_array(int **source, int **destination, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      destination[i][j] = source[i][j];
    }
  }
}

void copy_from_static_array(int (*source)[FIGURE_SIZE], int **destination,
                            int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      destination[i][j] = source[i][j];
    }
  }
}