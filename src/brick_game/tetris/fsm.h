#ifndef FSM_H
#define FSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "tetris.h"

funcPointer fsm(GameState_t state, UserAction_t action);

#ifdef __cplusplus
}
#endif

#endif