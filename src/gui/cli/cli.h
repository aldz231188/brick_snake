#ifndef CLI_H
#define CLI_H
#include "../../brick_game/common/common.h"
#include <ncurses.h>

#ifdef __cplusplus
extern "C" {
#endif

void print();
void setup();
void showField(WINDOW *field, int **_field);
void showInfo(WINDOW *info, GameInfo_t game);
void print();
UserAction_t get_input();

#ifdef __cplusplus
}
#endif

#endif
