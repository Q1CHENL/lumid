#include "TrayMenu.hpp"

#include <QProcess>

#include "MainWindow.hpp"
#include "PreferencesWindow.hpp"

TrayMenu::TrayMenu() {
    m_Open = std::make_unique<QAction>("Open Lumid", this);
    m_OpenDisplaySetting = std::make_unique<QAction>("Open display setting", this);
    m_Preferences = std::make_unique<QAction>("Preferences", this);
    m_Exit = std::make_unique<QAction>("Exit", this);

    addAction(m_Open.get());
    addAction(m_OpenDisplaySetting.get());
    addAction(m_Preferences.get());
    addAction(m_Exit.get());
}

void TrayMenu::connectSignals(MainWindow* mainWindow, PreferencesWindow* prefsWindow) {
    // use "=" to capture ptr to ensure to have a copy
    connect(m_Open.get(), &QAction::triggered, this, [=]() { mainWindow->showOnTopLeft(); });
    connect(m_OpenDisplaySetting.get(), &QAction::triggered, this, []() { QProcess::startDetached("gnome-control-center", QStringList() << "display"); });
    connect(m_Preferences.get(), &QAction::triggered, this, [=]() {  // catch pointer by value!
        prefsWindow->show();
        prefsWindow->raise();
    });
    connect(m_Exit.get(), &QAction::triggered, this, [=]() { mainWindow->onExit(); });
}
