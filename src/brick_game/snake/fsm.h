#ifndef FSM_H
#define FSM_H
#include "model.h"
typedef enum {
  Listen_State,
  Start_State,
  Play_State,
  Pause_State,
  GameOver_State,
  Terminate_State
} FSMState_t;

namespace s21 {
class GameParams_t;
class FSM {
 private:
  FSMState_t state;

 public:
  void apply_fsm(s21::GameParams_t *params);
  void change_fsm_state(FSMState_t new_state);
  FSMState_t get_fsm_state();
  void handle_fsm(UserAction_t action);
  FSM();
  ~FSM();
};
}  // namespace s21

#endif
