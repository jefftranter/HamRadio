#include <QtGui/QApplication>
#include "ToroidWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ToroidWindow w;
    w.show();
    return a.exec();
}
