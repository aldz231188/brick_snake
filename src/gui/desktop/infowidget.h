#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>

#include "../../brick_game/common/common.h"  // Заголовочный файл с определением GameInfo_t

class InfoWidget : public QWidget {
  Q_OBJECT

 public:
  explicit InfoWidget(QWidget *parent = nullptr);

  // Функция для обновления информации об игре
  void updateGameInfo(const GameInfo_t &gameInfo);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  GameInfo_t currentGameInfo;
};

#endif  // INFOWIDGET_H
