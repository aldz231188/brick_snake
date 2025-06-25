#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

#include "gamewidget.h"
#include "infowidget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QWidget mainWindow;
  GameWidget *gameWidget = new GameWidget();
  InfoWidget *infoWidget = new InfoWidget();

  // Установка политики размера
  gameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  infoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(gameWidget, 1);  // Даем GameWidget больше пространства
  layout->addWidget(infoWidget, 1);

  mainWindow.setLayout(layout);
  mainWindow.resize(600, 600);
  mainWindow.show();

  // Устанавливаем фокус на GameWidget
  gameWidget->setFocus();

  QObject::connect(gameWidget, &GameWidget::gameInfoUpdated, infoWidget,
                   &InfoWidget::updateGameInfo);

  return app.exec();
}
