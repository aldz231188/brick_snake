#include "model.h"

#include "../common/common.h"  // Для функций read_highscore и write_highscore
#include "fsm.h"

// Реализация методов класса snake_segment

s21::snake_segment::snake_segment(int y, int x, UserAction_t direction,
                                  UserAction_t prev_direction)
    : position(y, x), direction(direction), prev_direction(prev_direction) {}

std::pair<int, int> s21::snake_segment::define_next_direction() {
  const std::pair<int, int> offsets[] = {
      {0, -1},  // Left
      {0, 1},   // Right
      {-1, 0},  // Up
      {1, 0}    // Down
  };
  return offsets[direction - Left];
}

std::pair<int, int> s21::snake_segment::define_next_position() {
  std::pair<int, int> new_position;
  std::pair<int, int> new_direction = define_next_direction();
  new_position.first = position.first + new_direction.first;
  new_position.second = position.second + new_direction.second;
  return new_position;
}

void s21::snake_segment::moving() { this->position = define_next_position(); }

void s21::snake_segment::change_direction(UserAction_t new_direction) {
  switch (new_direction) {
    case Right:
      if (prev_direction != Left) {
        direction = new_direction;
      }
      break;
    case Left:
      if (prev_direction != Right) {
        direction = new_direction;
      }
      break;
    case Up:
      if (prev_direction != Down) {
        direction = new_direction;
      }
      break;
    case Down:
      if (prev_direction != Up) {
        direction = new_direction;
      }
      break;
    default:
      break;
  }
}

void s21::snake_segment::place_on_field(int **field) { field[y()][x()] = 1; }

int s21::snake_segment::y() { return this->position.first; }

int s21::snake_segment::x() { return this->position.second; }

// Реализация методов класса Apple

s21::Apple::Apple(const std::vector<snake_segment> &snake_body) {
  generate_apple(snake_body);
}

s21::Apple *s21::Apple::generate_apple(
    const std::vector<snake_segment> &snake_body) {
  std::random_device rd;
  std::mt19937 gen(rd());
  int lower_bound = 0;
  int upper_bound_X = FIELD_X - 1;
  int upper_bound_Y = FIELD_Y - 1;

  std::uniform_int_distribution<> distrib_Y(lower_bound, upper_bound_Y);
  std::uniform_int_distribution<> distrib_X(lower_bound, upper_bound_X);

  do {
    position = {distrib_Y(gen), distrib_X(gen)};
  } while (is_on_snake(snake_body));
  return this;
}

bool s21::Apple::is_on_snake(
    const std::vector<snake_segment> &snake_body) const {
  for (const auto &part : snake_body) {
    if (part.position == position) {
      return true;
    }
  }
  return false;
}

void s21::Apple::place_apple_on_field(int **field) const {
  field[y()][x()] = 1;
}

void s21::Apple::remove_apple_from_field(int **field) const {
  field[y()][x()] = 0;
}

int s21::Apple::y() const { return position.first; }

int s21::Apple::x() const { return position.second; }

// Реализация методов класса Snake

s21::Snake::Snake() {
  _snake.emplace_back(6, 3, Down, Down);
  _snake.emplace_back(5, 3, Down, Down);
  _snake.emplace_back(4, 3, Down, Down);
  _snake.emplace_back(3, 3, Down, Down);
}

bool s21::Snake::collision() {
  bool _collision = false;
  auto head = _snake.begin();
  auto head_next_position = head->define_next_position();
  if (head_next_position.first > FIELD_Y - 1 || head_next_position.first < 0 ||
      head_next_position.second > FIELD_X - 1 ||
      head_next_position.second < 0) {
    _collision = true;
  }
  for (auto next_part = head + 1; next_part != _snake.end() && !_collision;
       ++next_part) {
    if (head_next_position == next_part->define_next_position())
      _collision = true;
  }
  return _collision;
}

bool s21::Snake::make_step() {
  bool _collision = false;
  auto head = _snake.begin();
  for (auto it = head; it != _snake.end(); ++it) {
    if (it != _snake.end() - 1) (it + 1)->direction = it->prev_direction;
  }
  if (!collision()) {
    for (auto it = head; it != _snake.end(); ++it) {
      it->moving();
      it->prev_direction = it->direction;
    }
  } else
    _collision = true;
  return _collision;
}

void s21::Snake::eat_apple() {
  auto tail = --_snake.end();
  auto _direction = tail->define_next_direction();
  _snake.emplace_back(tail->y() - _direction.first,
                      tail->x() - _direction.second, tail->prev_direction,
                      tail->prev_direction);
}

void s21::Snake::change_direction(UserAction_t action) {
  _snake.begin()->change_direction(action);
}

void s21::Snake::place_snake_on_field(int **field) {
  for (auto it = _snake.begin(); it != _snake.end(); ++it) {
    it->place_on_field(field);
  }
}

void s21::Snake::remove_snake_from_field(int **field) {
  for (auto it = _snake.begin(); it != _snake.end(); ++it) {
    it->place_on_field(field);
  }
}

void s21::Snake::reset_snake() {
  _snake.clear();
  _snake.emplace_back(6, 3, Down, Down);
  _snake.emplace_back(5, 3, Down, Down);
  _snake.emplace_back(4, 3, Down, Down);
  _snake.emplace_back(3, 3, Down, Down);
}

// Реализация методов класса GameParams_t

s21::GameParams_t::GameParams_t() {
  fsm_state_ptr = std::make_unique<FSM>();
  current_state_ptr = std::make_unique<GameInfo_t>();
  snake_ptr = std::make_unique<Snake>();
  apple_ptr = std::make_unique<Apple>(snake_ptr->_snake);
  clock_gettime(CLOCK_REALTIME, &last_move_time);

  // Инициализация начального состояния игры
  init_start_state();
}

s21::GameParams_t *s21::GameParams_t::getInstance() {
  static GameParams_t instance;
  return &instance;
}

// bool s21::GameParams_t::time_for_step() {
//     struct timespec current_time;
//     clock_gettime(CLOCK_REALTIME, &current_time);
//     // static clock_t last_processing_time = clock();
//     double elapsed_time = (double)(current_time.tv_nsec -
//     last_move_time.tv_nsec); if (elapsed_time >= 1 *
//     ((current_state_ptr->speed - current_state_ptr->level) / (double)10)) {
//         last_move_time = current_time;
//         return true;
//     } else
//         return false;

// }
bool s21::GameParams_t::time_for_step() {
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  double elapsed_time = (current_time.tv_sec - last_move_time.tv_sec) +
                        (current_time.tv_nsec - last_move_time.tv_nsec) / 1e9;
  double interval =
      current_state_ptr->speed /
      ((current_state_ptr->level * 0.5 + 0.4) * (current_state_ptr->speed));
  if (elapsed_time >= interval) {
    last_move_time = current_time;
    return true;
  } else {
    return false;
  }
}

bool s21::GameParams_t::make_step() {
  bool _collision = false;
  _collision = snake_ptr->make_step();
  if (apple_ptr->is_on_snake(snake_ptr->_snake)) {
    snake_ptr->eat_apple();
    apple_ptr->generate_apple(snake_ptr->_snake);
    score_count();
  }
  return _collision;
}

void s21::GameParams_t::score_count() {
  (current_state_ptr->score)++;
  if (current_state_ptr->score % SCORE_THRESHOLD == 0)
    if (current_state_ptr->level <= MAX_LVL) (current_state_ptr->level)++;
  if (current_state_ptr->score > current_state_ptr->high_score) {
    current_state_ptr->high_score = current_state_ptr->score;
    write_highscore(current_state_ptr->score);
  }
  if (current_state_ptr->high_score == MAX_SCORE) restart();
}

void s21::GameParams_t::init_start_state() {
  current_state_ptr->field = allocate_array(FIELD_Y, FIELD_X);
  current_state_ptr->score = 0;
  read_highscore(&current_state_ptr->high_score);
  current_state_ptr->level = 1;
  current_state_ptr->speed = START_SPEED;
  current_state_ptr->pause = 0;
}

void s21::GameParams_t::place_apple_and_snake_on_field() {
  snake_ptr.get()->place_snake_on_field(current_state_ptr.get()->field);
  apple_ptr.get()->place_apple_on_field(current_state_ptr.get()->field);
}

void s21::GameParams_t::restart() {
  // if (current_state_ptr->score >= current_state_ptr->high_score)
  //     write_highscore(current_state_ptr->score);
  reset_array();
  snake_ptr->reset_snake();
  apple_ptr->generate_apple(snake_ptr->_snake);
  current_state_ptr->score = 0;
  read_highscore(&current_state_ptr->high_score);
  current_state_ptr->level = 1;
  current_state_ptr->speed = START_SPEED;
  current_state_ptr->pause = 0;
}

void s21::GameParams_t::terminate() {
  // if (current_state_ptr->score >= current_state_ptr->high_score)
  //     write_highscore(current_state_ptr->score);
  deallocate_array();
}

int **s21::GameParams_t::allocate_array(int nRows, int nCols) {
  int **array = new int *[nRows];
  for (int i = 0; i < nRows; ++i) array[i] = new int[nCols]();
  return array;
}

void s21::GameParams_t::deallocate_array() {
  for (int i = 0; i < FIELD_Y; ++i) {
    delete[] current_state_ptr->field[i];
  }
  delete[] current_state_ptr->field;
}

void s21::GameParams_t::reset_array() {
  if (current_state_ptr->field) {
    for (int i = 0; i < FIELD_Y; ++i) {
      for (int j = 0; j < FIELD_X; ++j) {
        current_state_ptr->field[i][j] = 0;
      }
    }
  }
}

GameInfo_t *s21::GameParams_t::getCurrentState() {
  return current_state_ptr.get();
}

s21::FSM *s21::GameParams_t::getFsm() { return fsm_state_ptr.get(); }

s21::Snake *s21::GameParams_t::getSnake() { return snake_ptr.get(); }

s21::Apple *s21::GameParams_t::getApple() { return apple_ptr.get(); }
