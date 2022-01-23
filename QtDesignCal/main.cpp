#include "qtdesigncal.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtDesignCal w;
    w.resize(750, 530);
    w.show();
    return a.exec();
}

