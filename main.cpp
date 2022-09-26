#include "Serial_port.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Serial_port w;
    w.show();

    return a.exec();
}
