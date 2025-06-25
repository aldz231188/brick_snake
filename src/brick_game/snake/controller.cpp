
#include "controller.h"

#include "model.h"

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  s21::GameParams_t *params = s21::GameParams_t::getInstance();
  if (action >= Start && action <= Terminate) {
    params->getFsm()->handle_fsm(action);
  } else if (action == Action) {
    if (params->getFsm()->get_fsm_state() == Play_State) {
      params->make_step();
    }
  } else if (action >= Left && action <= Down) {
    if (params->getFsm()->get_fsm_state() == Play_State) {
      params->getSnake()->change_direction(action);
    }
  }
}