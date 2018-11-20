#include "Gefest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gefest w;
    w.show();

    return a.exec();
}
