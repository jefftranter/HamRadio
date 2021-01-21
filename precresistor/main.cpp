#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/images/circuit-sp3a.png"));
  MainWindow w;
  w.show();
  return a.exec();
}
