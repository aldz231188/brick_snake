#include "figures.h"

int (*random_figure_generator())[FIGURE_SIZE] {
  srand(time(0));
  static int figure_patterns[FIGURE_COUNT][FIGURE_SIZE][FIGURE_SIZE] = {0};
  static int initialized = 0;
  if (!initialized) {
    initialized = 1;
    // Фигура 1 - I
    figure_patterns[0][2][0] = 2, figure_patterns[0][2][1] = 2,
    figure_patterns[0][2][2] = 2, figure_patterns[0][2][3] = 2;
    // Фигура 2 - J
    figure_patterns[1][1][1] = 2, figure_patterns[1][2][1] = 2,
    figure_patterns[1][2][2] = 2, figure_patterns[1][2][3] = 2;
    // Фигура 3 - L
    figure_patterns[2][1][3] = 2, figure_patterns[2][2][1] = 2,
    figure_patterns[2][2][2] = 2, figure_patterns[2][2][3] = 2;
    // Фигура 4 - O
    figure_patterns[3][1][1] = 2, figure_patterns[3][1][2] = 2,
    figure_patterns[3][2][1] = 2, figure_patterns[3][2][2] = 2;
    // Фигура 5 - S
    figure_patterns[4][1][2] = 2, figure_patterns[4][1][3] = 2,
    figure_patterns[4][2][1] = 2, figure_patterns[4][2][2] = 2;
    // Фигура 6 - T
    figure_patterns[5][1][2] = 2, figure_patterns[5][2][1] = 2,
    figure_patterns[5][2][2] = 2, figure_patterns[5][2][3] = 2;
    // Фигура 7 - Z
    figure_patterns[6][1][1] = 2, figure_patterns[6][1][2] = 2,
    figure_patterns[6][2][2] = 2, figure_patterns[6][2][3] = 2;
  }
  return figure_patterns[rand() % FIGURE_COUNT];
}
