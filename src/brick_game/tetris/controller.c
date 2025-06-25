#include "controller.h"

#include "fsm.h"

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  GameParams_t *params = params_holder();
  funcPointer func = NULL;
  if (action > Action) {
    action = Up;
  }
  func = fsm(*params->state, action);
  if (func) func(params);
}
