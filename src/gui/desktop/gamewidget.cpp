#include "gamewidget.h"

#include <QDebug>
#include <QPainter>
#include <QTimer>

#include "../../brick_game/common/common.h"
#include "../../brick_game/snake/controller.h"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);

  setMinimumSize(300, 600);
  setMaximumSize(600, 1200);
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GameWidget::gameLoop);
  timer->start(30);
}

void GameWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);

  // Установка цвета фона
  painter.fillRect(rect(), Qt::black);

  // Размеры клетки
  int cellWidth = width() / 10;
  int cellHeight = height() / 20;
  // Отрисовка игрового поля
  if (gameInfo.field)
    for (int y = 0; y < FIELD_Y; ++y) {
      for (int x = 0; x < FIELD_X; ++x) {
        if (gameInfo.field[y][x]) {
          painter.setBrush(Qt::green);
          painter.drawRect(x * cellWidth, y * cellHeight, cellWidth,
                           cellHeight);
        }
      }
    }
}

void GameWidget::gameLoop() {
  // qDebug() << "gameLoop called";
  // Обновление состояния игры
  gameInfo = updateCurrentState();

  // Излучаем сигнал с обновленной информацией об игре
  emit gameInfoUpdated(gameInfo);
  // Перерисовка виджета
  update();
}

#include <QApplication>
#include <QKeyEvent>

void GameWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_S:
      userInput(Start, 0);
      break;
    case Qt::Key_Space:
      userInput(Pause, 0);
      break;
    case Qt::Key_Q:
      userInput(Terminate, 0);
      QApplication::quit();
      break;
    case Qt::Key_Left:
      userInput(Left, 0);
      break;
    case Qt::Key_Right:
      userInput(Right, 0);
      break;
    case Qt::Key_Up:
      userInput(Up, 0);
      break;
    case Qt::Key_Down:
      userInput(Down, 0);
      break;
    case Qt::Key_A:
      userInput(Action, 0);
      break;
    default:
      userInput((UserAction_t)-1, 0);
      break;
  }
}

