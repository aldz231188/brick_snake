#include "fsm.h"
funcPointer fsm(GameState_t state, UserAction_t action) {
  funcPointer fsmTable[6][8] = {
      {start_game, NULL, terminate, NULL, NULL, NULL, NULL, NULL},  // Start
      {NULL, pause_game, terminate, NULL, NULL, NULL, NULL, NULL},  // Pause
      {spawn_figure, spawn_figure, spawn_figure, spawn_figure, spawn_figure,
       spawn_figure, spawn_figure, spawn_figure},  // Spawn
      {NULL, pause_game, terminate, move_left, move_right, NULL, move_down,
       rotate_figure},  // Game
      {attaching, attaching, attaching, attaching, attaching, attaching,
       attaching, attaching},  // Attach
      {restart_game, NULL, terminate, NULL, NULL, NULL, NULL,
       NULL},  // Gameover
  };
  return fsmTable[state][action];
}
