#ifndef FIGURES_H
#define FIGURES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "tetris.h"

int (*random_figure_generator())[FIGURE_SIZE];

#ifdef __cplusplus
}
#endif

#endif