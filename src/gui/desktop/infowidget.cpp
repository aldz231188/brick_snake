#include "infowidget.h"

#include <QPainter>

InfoWidget::InfoWidget(QWidget *parent) : QWidget(parent) {}

void InfoWidget::updateGameInfo(const GameInfo_t &gameInfo) {
  currentGameInfo = gameInfo;
  update();  // Перерисовываем виджет
}

void InfoWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);

  // Устанавливаем цвет фона
  painter.fillRect(rect(), Qt::black);

  // Устанавливаем цвет текста
  painter.setPen(Qt::white);

  // Отображаем информацию об игре
  int y = 20;  // Начальная позиция по вертикали
  int lineHeight = 20;  // Высота между строками

  painter.drawText(10, y,
                   QString("High Score: %1").arg(currentGameInfo.high_score));
  y += lineHeight;
  painter.drawText(10, y, QString("Score: %1").arg(currentGameInfo.score));
  y += lineHeight;
  painter.drawText(10, y, QString("Level: %1").arg(currentGameInfo.level));
  y += lineHeight;

  // Отображаем состояние паузы или окончания игры
  if (currentGameInfo.pause == 1) {
    painter.drawText(10, y, "PAUSE");
    y += lineHeight;
  } else if (currentGameInfo.pause == 2) {
    painter.drawText(10, y, "GAME OVER!");
    y += lineHeight;
  }

  // Если это Тетрис, отображаем следующую фигуру
  if (currentGameInfo.next) {
    y += lineHeight;
    painter.drawText(10, y, "Next:");
    y += lineHeight;

    // Отрисовка следующей фигуры
    int cellSize = 20;  // Размер клетки
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        if (currentGameInfo.next[i][j]) {
          painter.setBrush(Qt::green);
          painter.drawRect(10 + j * cellSize, y + i * cellSize, cellSize,
                           cellSize);
        }
      }
    }
  }
}
