
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
