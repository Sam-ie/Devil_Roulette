#include "startup.h"
#include "music.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Music::getInstance()->setVolume(50);
    Startup w;
    w.show();
    return a.exec();
}
