#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  gameWidget = new GameWidget(this);
  setCentralWidget(gameWidget);
  setWindowTitle("Snake Game");
  // resize(400, 800); // Установите желаемый размер окна
}
