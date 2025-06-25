#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define FIELD_Y 20
#define FIELD_X 10

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

void read_highscore(int *high_score);

void write_highscore(int high_score);

#ifdef __cplusplus
}
#endif

#endif