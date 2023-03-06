#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    MainWindow window;
    window.show();

    return QApplication::exec();
}
//todo solve bug when multiple at once
//todo do disapear when mouse clicked
//todo custimize keyboard shortcuts