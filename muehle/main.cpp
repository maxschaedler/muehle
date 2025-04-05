#include "logic.h"
#include "gui.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    muehle w;
    w.show();

    return a.exec();
}
