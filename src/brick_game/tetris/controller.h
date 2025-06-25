#ifndef CONTROLLER_H
#define CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "tetris.h"

void userInput(UserAction_t action, bool hold);

#ifdef __cplusplus
}
#endif

#endif