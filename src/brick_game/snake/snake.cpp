#include "../../gui/cli/cli.h"
#include "fsm.h"
#include "model.h"

GameInfo_t updateCurrentState() {
  s21::GameParams_t *params = s21::GameParams_t::getInstance();
  params->reset_array();
  params->getFsm()->apply_fsm(params);
  if (params->getFsm()->get_fsm_state() != Terminate_State) {
    params->place_apple_and_snake_on_field();
  }
  return *params->getCurrentState();
}
