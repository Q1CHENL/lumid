#ifndef LUMID_TRAYMENU_HPP
#define LUMID_TRAYMENU_HPP

#include <QAction>
#include <QMenu>

#include "MainWindow.hpp"
#include "PreferencesWindow.hpp"

class TrayMenu : public QMenu {
   public:
    TrayMenu();

    std::unique_ptr<QAction> m_Open;

    std::unique_ptr<QAction> m_OpenDisplaySetting;

    std::unique_ptr<QAction> m_Preferences;

    std::unique_ptr<QAction> m_Exit;

    void connectSignals(MainWindow* mainWindow, PreferencesWindow* prefsWindow);
};

#endif