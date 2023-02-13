
#include <QApplication>
#include "Slider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Slider window;
    window.show();

    return QApplication::exec();
}
//todo show current value
//todo set brightness with current value instead of constant 10