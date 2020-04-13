#include "stringalgorithms.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StringAlgorithms w;
    w.show();

    return a.exec();
}
