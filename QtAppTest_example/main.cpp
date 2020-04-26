#include <QApplication>
#include <QSystemTrayIcon>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(resources);

  QApplication MyApp(argc, argv);

  MainWindow MyWindow;
  MyWindow.show();

  return MyApp.exec();
}
