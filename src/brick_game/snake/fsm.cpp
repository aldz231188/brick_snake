#include "fsm.h"

#include "model.h"

s21::FSM::FSM() : state(Start_State) {}

s21::FSM::~FSM() {}

void s21::FSM::apply_fsm(s21::GameParams_t *params) {
  bool _collision = false;
  switch (state) {
    case Listen_State:
      break;
    case Start_State:
      break;
    case Play_State:
      if (params->time_for_step()) _collision = params->make_step();
      if (_collision) {
        state = GameOver_State;
      }
      break;
    case Pause_State:
      if (params->getCurrentState()->pause) {
        params->getCurrentState()->pause = 0;
        change_fsm_state(Play_State);
      } else {
        params->getCurrentState()->pause = 1;
        change_fsm_state(Listen_State);
      }
      break;
    case GameOver_State:
      params->restart();
      change_fsm_state(Start_State);
      break;
    case Terminate_State:
      params->terminate();
      params->getCurrentState()->pause = 3;
      break;
    default:
      break;
  }
}

void s21::FSM::handle_fsm(UserAction_t action) {
  switch (action) {
    case Start:
      if (get_fsm_state() == Start_State) {
        change_fsm_state(Play_State);
      }
      break;
    case Pause:
      if (get_fsm_state() == Listen_State || get_fsm_state() == Play_State) {
        change_fsm_state(Pause_State);
      }
      break;
    case Terminate:
      change_fsm_state(Terminate_State);
      break;
    default:
      break;
  }
}

void s21::FSM::change_fsm_state(FSMState_t new_state) {
  if (new_state >= Listen_State || new_state <= Terminate_State)
    state = new_state;
}
FSMState_t s21::FSM::get_fsm_state() { return state; }
