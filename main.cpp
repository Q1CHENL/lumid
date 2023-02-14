
#include <QApplication>
#include "Sliders.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Sliders window;
    window.show();

    return QApplication::exec();
}
//todo show current value
//todo set brightness with current value instead of constant 10
//todo do not exit but to tray
//todo add desktop entry
//todo solve bug when multiple at once
//todo add second layout including slider and value for the second display