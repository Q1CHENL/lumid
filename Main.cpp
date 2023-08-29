#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    // window.showOnTopLeft();

    return QApplication::exec();
}
// todo custimize keyboard shortcuts