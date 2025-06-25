#ifndef SNAKE_H
#define SNAKE_H

#include <memory>
#include <random>
#include <utility>  // Для std::pair
#include <vector>

#include "controller.h"
#include "fsm.h"

#define MAX_LVL 10
#define START_SPEED 10
#define SCORE_THRESHOLD 5
#define MAX_SCORE 200

namespace s21 {
class snake_segment {
 public:
  std::pair<int, int> position;
  UserAction_t direction;
  UserAction_t prev_direction;

  snake_segment(int y, int x, UserAction_t direction,
                UserAction_t prev_direction);

  std::pair<int, int> define_next_direction();
  std::pair<int, int> define_next_position();
  void moving();
  void change_direction(UserAction_t new_direction);
  void place_on_field(int **field);
  int y();
  int x();
};

class Apple {
 public:
  std::pair<int, int> position;
  Apple(const std::vector<snake_segment> &snake_body);
  Apple *generate_apple(const std::vector<snake_segment> &snake_body);
  bool is_on_snake(const std::vector<snake_segment> &snake_body) const;
  void place_apple_on_field(int **field) const;
  void remove_apple_from_field(int **field) const;
  int y() const;
  int x() const;
};

class Snake {
 public:
  std::vector<snake_segment> _snake;
  Snake();
  bool collision();
  bool make_step();
  void eat_apple();
  void change_direction(UserAction_t action);
  void place_snake_on_field(int **field);
  void remove_snake_from_field(int **field);
  void reset_snake();
};
class FSM;
class GameParams_t {
 private:
  std::unique_ptr<FSM> fsm_state_ptr;
  std::unique_ptr<GameInfo_t> current_state_ptr;
  std::unique_ptr<Snake> snake_ptr;
  std::unique_ptr<Apple> apple_ptr;
  struct timespec last_move_time;
  GameParams_t();

 public:
  static GameParams_t *getInstance();
  GameParams_t(const GameParams_t &) = delete;
  GameParams_t &operator=(const GameParams_t &) = delete;
  bool time_for_step();
  bool make_step();
  void score_count();
  void init_start_state();
  void place_apple_and_snake_on_field();
  void restart();
  void terminate();
  int **allocate_array(int nRows, int nCols);
  void deallocate_array();
  void reset_array();
  GameInfo_t *getCurrentState();
  FSM *getFsm();
  Snake *getSnake();
  Apple *getApple();
};
}  // namespace s21

#endif  // SNAKE_H
