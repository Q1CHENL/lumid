
#include <QApplication>
#include "Sliders.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Sliders window;
    window.show();

    return QApplication::exec();
}
//todo solve bug when multiple at once
//todo simplify constructor of Sliders
//todo do disapear when mouse clicked
//todo custimize keyboard shortcuts
//todo dynamically initialize sliders using tuple or other data structures
