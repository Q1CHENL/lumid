#include <QApplication>
#include <QObject>
#include <QSystemTrayIcon>

#include "MainWindow.hpp"
#include "PreferencesWindow.hpp"
#include "TrayMenu.hpp"

/**
 * @brief
 * Here we initilize our program and handle all external
 * couplings between MainWindow, TrayIcon, TrayMenu and
 * PreferencesWindow Objects.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    QSystemTrayIcon trayIcon;
    TrayMenu trayMenu;
    PreferencesWindow preferencesWindow;

    // set up tray menu
    trayMenu.connectSignals(&mainWindow, &preferencesWindow);
    // trayMenu.setParent(&mainWindow);
    preferencesWindow.setWindowFlags(preferencesWindow.windowFlags() | Qt::WindowStaysOnTopHint);

    // set up tray icon
    // trayIcon.setIcon(QIcon("/usr/share/icons/lumid.png"));
    trayIcon.setIcon(QIcon("../assets/icon.png")); // for debug/run in vscode, the path might be "assets/icon.png", it depends on the launch.json
    trayIcon.setContextMenu(&trayMenu);
    trayIcon.show();

    // must catch by reference since copy constructor is deleted in Qt
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, &mainWindow,
                     [&](QSystemTrayIcon::ActivationReason reason) {
                         if (reason == QSystemTrayIcon::Trigger) {
                             mainWindow.m_Timer.stop();
                             mainWindow.showOnTopLeft();
                             restartTimerForSecs(&(mainWindow.m_Timer), STAY_TIME_LONG);
                         }
                     });

    QObject::connect(preferencesWindow.applyButton, &QPushButton::clicked, &preferencesWindow, [&]() {
        mainWindow.setStride(preferencesWindow.spinbox->value());

        std::unique_ptr<QKeySequence> increaseSeq = std::make_unique<QKeySequence>(preferencesWindow.keySeqEditIncrease->keySequence());
        std::unique_ptr<QKeySequence> decreaseSeq = std::make_unique<QKeySequence>(preferencesWindow.keySeqEditDecrease->keySequence());

        mainWindow.setShortcuts(
            std::make_unique<QKeySequence>(preferencesWindow.keySeqEditIncrease->keySequence()).get(),
            std::make_unique<QKeySequence>(preferencesWindow.keySeqEditDecrease->keySequence()).get());

        // don't know why this makes the program quit, maybe because qt thinks that the prefs is the last window
        // preferencesWindow.accept();
        preferencesWindow.hide();
    });

    QObject::connect(preferencesWindow.resetButton, &QPushButton::clicked, &preferencesWindow, [&]() {
        // reset stride
        preferencesWindow.spinbox->setValue(10);
        mainWindow.setStride(10);

        // reset shortcuts
        preferencesWindow.keySeqEditIncrease->setKeySequence(*(std::make_unique<QKeySequence>(Qt::Key_F6).get()));
        preferencesWindow.keySeqEditDecrease->setKeySequence(*(std::make_unique<QKeySequence>(Qt::Key_F5).get()));
        QKeySequence keySeqIn = preferencesWindow.keySeqEditIncrease->keySequence();
        QKeySequence keySeqDe = preferencesWindow.keySeqEditDecrease->keySequence();
        mainWindow.setShortcuts(&keySeqIn, &keySeqDe);
    });

    // window.showOnTopLeft();

    return QApplication::exec();
}
