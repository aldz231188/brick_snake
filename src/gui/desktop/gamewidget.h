#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>

#include "../../brick_game/common/common.h"  // Заголовочный файл с определением GameInfo_t

class GameWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GameWidget(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  GameInfo_t gameInfo;

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void gameLoop();

 signals:
  void gameInfoUpdated(const GameInfo_t &gameInfo);
};

#endif  // GAMEWIDGET_H
