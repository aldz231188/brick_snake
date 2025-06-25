#include <gtest/gtest.h>

#include "model.h"

// test_snake_game.cpp

#include <gtest/gtest.h>

#include "../common/common.h"
#include "fsm.h"
#include "model.h"

namespace s21 {

// Тестирование класса snake_segment
TEST(SnakeSegmentTest, MovementTest) {
  snake_segment segment(5, 5, Right, Right);
  segment.moving();
  EXPECT_EQ(segment.x(), 6);
  EXPECT_EQ(segment.y(), 5);

  segment.change_direction(Up);
  segment.moving();
  EXPECT_EQ(segment.x(), 6);
  EXPECT_EQ(segment.y(), 4);
}

TEST(SnakeSegmentTest, ChangeDirectionTest) {
  snake_segment segment(5, 5, Right, Right);

  // Попытка повернуть в противоположную сторону (должна игнорироваться)
  segment.change_direction(Left);
  EXPECT_EQ(segment.direction, Right);

  // Поворот вверх (должен измениться)
  segment.change_direction(Up);
  EXPECT_EQ(segment.direction, Up);
}

// Тестирование класса Apple
TEST(AppleTest, GenerateAppleTest) {
  std::vector<snake_segment> snake_body = {snake_segment(5, 5, Right, Right),
                                           snake_segment(5, 4, Right, Right),
                                           snake_segment(5, 3, Right, Right)};
  Apple apple(snake_body);

  // Проверяем, что яблоко не генерируется на теле змейки
  for (const auto& segment : snake_body) {
    EXPECT_FALSE(segment.position == apple.position);
  }
}

TEST(AppleTest, IsOnSnakeTest) {
  std::vector<snake_segment> snake_body = {snake_segment(5, 5, Right, Right)};
  Apple apple(snake_body);

  // Принудительно устанавливаем яблоко на позицию змейки
  apple.position = {5, 5};
  EXPECT_TRUE(apple.is_on_snake(snake_body));
}

// Тестирование класса Snake
TEST(SnakeTest, MakeStepTest) {
  Snake snake;
  auto initial_head_position = snake._snake.front().position;

  snake.make_step();
  auto new_head_position = snake._snake.front().position;

  // Проверяем, что голова змейки переместилась вниз (по умолчанию)
  EXPECT_EQ(new_head_position.first, initial_head_position.first + 1);
  EXPECT_EQ(new_head_position.second, initial_head_position.second);
}

TEST(SnakeTest, EatAppleTest) {
  Snake snake;
  size_t initial_length = snake._snake.size();

  snake.eat_apple();
  size_t new_length = snake._snake.size();

  // Проверяем, что длина змейки увеличилась на 1
  EXPECT_EQ(new_length, initial_length + 1);
}

TEST(SnakeTest, CollisionTest) {
  Snake snake;

  // Принудительно создаем ситуацию столкновения с границей
  snake._snake.front().position = {FIELD_Y - 1, FIELD_X / 2};
  snake._snake.front().direction = Down;

  bool collision = snake.collision();
  EXPECT_TRUE(collision);
}

// Тестирование класса GameParams_t
TEST(GameParamsTest, ScoreCountTest) {
  GameParams_t* game = GameParams_t::getInstance();
  int initial_score = game->getCurrentState()->score;
  int initial_level = game->getCurrentState()->level;

  // Имитируем набор очков
  for (int i = 0; i < SCORE_THRESHOLD; ++i) {
    game->score_count();
  }

  int new_score = game->getCurrentState()->score;
  int new_level = game->getCurrentState()->level;

  // Проверяем, что счет увеличился на SCORE_THRESHOLD
  EXPECT_EQ(new_score, initial_score + SCORE_THRESHOLD);

  // Проверяем, что уровень увеличился на 1
  EXPECT_EQ(new_level, initial_level + 1);
}

// TEST(GameParamsTest, TimeForStepTest) {
//     GameParams_t* game = GameParams_t::getInstance();

//     // Проверяем, что метод возвращает true после заданного интервала
//     bool time_step = game->time_for_step();
//     EXPECT_TRUE(time_step);
// }

TEST(GameParamsTest, RestartTest) {
  GameParams_t* game = GameParams_t::getInstance();
  game->getCurrentState()->score = 100;
  game->restart();

  // Проверяем, что счет сбросился
  EXPECT_EQ(game->getCurrentState()->score, 0);

  // Проверяем, что уровень сбросился
  EXPECT_EQ(game->getCurrentState()->level, 1);
}

TEST(SnakeSegmentTest, InvalidDirectionChangeTest) {
  snake_segment segment(5, 5, Right, Right);

  // Попытка установить некорректное направление
  segment.change_direction(static_cast<UserAction_t>(999));
  EXPECT_EQ(segment.direction, Right);
}

// Дополнительные тесты для класса Apple

TEST(AppleTest, AppleNotOnSnakeAfterGeneration) {
  std::vector<snake_segment> snake_body = {snake_segment(2, 2, Right, Right),
                                           snake_segment(2, 3, Right, Right),
                                           snake_segment(2, 4, Right, Right)};
  Apple apple(snake_body);

  // Проверяем, что яблоко не появилось на позиции змейки
  for (const auto& segment : snake_body) {
    EXPECT_NE(apple.position, segment.position);
  }
}

TEST(AppleTest, ApplePositionWithinField) {
  std::vector<snake_segment> snake_body;
  Apple apple(snake_body);

  // Проверяем, что яблоко находится внутри границ поля
  EXPECT_GE(apple.x(), 0);
  EXPECT_LT(apple.x(), FIELD_X);
  EXPECT_GE(apple.y(), 0);
  EXPECT_LT(apple.y(), FIELD_Y);
}

// Дополнительные тесты для класса Snake

TEST(SnakeTest, SelfCollisionTest) {
  Snake snake;

  // Создаем ситуацию, когда змейка сталкивается с собой
  snake._snake.clear();
  snake._snake.emplace_back(6, 5, Down, Down);
  snake._snake.emplace_back(5, 5, Down, Down);
  snake._snake.emplace_back(4, 5, Down, Down);
  snake._snake.emplace_back(3, 5, Down, Down);
  snake._snake.emplace_back(2, 5, Down, Down);

  snake.change_direction(Right);
  bool collision = snake.make_step();
  snake.change_direction(Up);
  collision = snake.make_step();
  snake.change_direction(Left);
  collision = snake.make_step();
  EXPECT_TRUE(collision);
}
TEST(SnakeTest, BoundaryCollisionTest) {
  Snake snake;

  // Создаем ситуацию, когда змейка сталкивается со стеной
  snake._snake.clear();
  snake._snake.emplace_back(4, 0, Down, Down);
  snake._snake.emplace_back(3, 0, Down, Down);
  snake._snake.emplace_back(2, 0, Down, Down);
  snake._snake.emplace_back(1, 0, Down, Down);
  snake.change_direction(Left);
  bool collision = snake.make_step();
  EXPECT_TRUE(collision);
}

TEST(SnakeTest, WrongDirectionTest) {
  // Создаем ситуацию, кода движение направлено в обратную сторону
  Snake snake;
  snake._snake.clear();
  snake._snake.emplace_back(4, 0, Down, Down);
  snake._snake.emplace_back(3, 0, Down, Down);
  snake._snake.emplace_back(2, 0, Down, Down);
  snake._snake.emplace_back(1, 0, Down, Down);
  snake.change_direction(Up);
  bool collision = snake.make_step();
  EXPECT_FALSE(collision);
  EXPECT_EQ(snake._snake.begin()->direction, Down);
}
TEST(SnakeTest, ControllerTest) {
  GameParams_t* game = GameParams_t::getInstance();
  userInput(Start, false);
  game->make_step();
  userInput(Right, false);
  EXPECT_EQ(game->getSnake()->_snake.begin()->direction, Right);
}
TEST(SnakeTest, updateCurrentStateTest) {
  GameParams_t* game = GameParams_t::getInstance();
  int temp[FIELD_Y][FIELD_X];
  bool progress = false;
  for (int i = 0; i < FIELD_Y; ++i) {
    for (int j = 0; j < FIELD_X; ++j) {
      temp[i][j] = game->getCurrentState()->field[i][j];
    }
  }

  game->make_step();
  updateCurrentState();
  for (int i = 0; i < FIELD_Y; ++i) {
    for (int j = 0; j < FIELD_X; ++j) {
      if (temp[i][j] == game->getCurrentState()->field[i][j])
        continue;
      else
        progress = true;
    }
  }
  EXPECT_TRUE(progress);
}

TEST(SnakeTest, GrowAfterEatingAppleTest) {
  Snake snake;
  size_t initial_length = snake._snake.size();

  // Имитируем поедание яблока
  snake.eat_apple();

  // Проверяем, что новый сегмент добавлен в конец змейки
  EXPECT_EQ(snake._snake.size(), initial_length + 1);
  auto tail = snake._snake.back();
  auto before_tail = *(snake._snake.end() - 2);
  EXPECT_NE(tail.position, before_tail.position);
}

// Дополнительные тесты для класса GameParams_t

TEST(GameParamsTest, PauseFunctionalityTest) {
  GameParams_t* game = GameParams_t::getInstance();

  // Имитируем паузу
  game->getCurrentState()->pause = 1;
  bool time_step = game->time_for_step();

  // Проверяем, что игра не делает шаг во время паузы
  EXPECT_FALSE(time_step);
}

// Дополнительные тесты для класса FSM

TEST(FSMTest, StateTransitionTest) {
  GameParams_t* game = GameParams_t::getInstance();

  game->getFsm()->handle_fsm(Pause);
  EXPECT_EQ(game->getFsm()->get_fsm_state(), Pause_State);
  game->getFsm()->handle_fsm(Pause);
  game->getFsm()->apply_fsm(game);
  EXPECT_EQ(game->getFsm()->get_fsm_state(), Play_State);
  game->getFsm()->change_fsm_state(GameOver_State);
  game->getFsm()->apply_fsm(game);
  EXPECT_EQ(game->getFsm()->get_fsm_state(), Start_State);
  game->getFsm()->handle_fsm(Start);
  game->getFsm()->apply_fsm(game);
  EXPECT_EQ(game->getFsm()->get_fsm_state(), Play_State);
  game->getFsm()->handle_fsm((UserAction_t)-1);
  game->getFsm()->apply_fsm(game);
  EXPECT_EQ(game->getFsm()->get_fsm_state(), Play_State);
  game->getFsm()->handle_fsm(Terminate);
  game->getFsm()->apply_fsm(game);
  EXPECT_EQ(game->getFsm()->get_fsm_state(), Terminate_State);
}

// Тестирование функции allocate_array и deallocate_array

TEST(GameParamsTest, AllocateDeallocateArrayTest) {
  GameParams_t* game = GameParams_t::getInstance();
  int** field = game->allocate_array(10, 10);

  // Проверяем, что массив был успешно создан
  ASSERT_NE(field, nullptr);
  for (int i = 0; i < 10; ++i) {
    ASSERT_NE(field[i], nullptr);
  }

  // Освобождаем память
  for (int i = 0; i < 10; ++i) {
    delete[] field[i];
  }
  delete[] field;
}

// Тестирование функций place_apple_on_field и place_snake_on_field

TEST(GameParamsTest, PlaceAppleAndSnakeOnFieldTest) {
  GameParams_t* game = GameParams_t::getInstance();
  game->reset_array();

  game->place_apple_and_snake_on_field();

  // Проверяем, что яблоко и змейка размещены на поле
  int count = 0;
  for (int i = 0; i < FIELD_Y; ++i) {
    for (int j = 0; j < FIELD_X; ++j) {
      if (game->getCurrentState()->field[i][j] == 1) {
        count++;
      }
    }
  }
  EXPECT_EQ(count, 5);
}

// Тестирование метода reset_snake

TEST(SnakeTest, ResetSnakeTest) {
  Snake snake;
  snake.eat_apple();
  size_t length_before_reset = snake._snake.size();

  snake.reset_snake();
  size_t length_after_reset = snake._snake.size();

  // Проверяем, что длина змейки вернулась к первоначальной
  EXPECT_LT(length_after_reset, length_before_reset);
  EXPECT_EQ(length_after_reset, 4);
}

}  // namespace s21

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
