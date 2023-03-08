//
// Created by liuqichen on 3/4/23.
//

#ifndef LUMID_WRAPPERS_HPP
#define LUMID_WRAPPERS_HPP

#include <QAction>
#include <QDialog>
#include <QMenu>
#include <QPushButton>
#include <QSlider>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QHBoxLayout>
#include <QLabel>

//include leads to circular dependency
class MainWindow;

namespace Wrappers {

class BrightnessSlider : public QSlider {
   public:
    QTimer *m_Timer{};

    BrightnessSlider() = default;

    void setTimer(QTimer *timer);
};

class ViewChangeButton : public QPushButton {
   public:
    QTimer *m_Timer{};

    ViewChangeButton() = default;

    void setTimer(QTimer *timer);
};

class PreferencesWindow : public QDialog {
   public:
    // todo add things
    // stride, bar, shortcuts...
    PreferencesWindow();
};

class TrayMenu : public QMenu {
   public:
    TrayMenu();

    std::unique_ptr<PreferencesWindow> m_PreferencesWindow =
        std::make_unique<PreferencesWindow>();
    std::unique_ptr<QAction> m_Open = std::make_unique<QAction>("Open", this);

    std::unique_ptr<QAction> m_Preferences =
        std::make_unique<QAction>("Preferences", this);

    std::unique_ptr<QAction> m_Exit = std::make_unique<QAction>("Exit", this);

    void connectSignals(MainWindow* mainWindow);

};

class SlidersHBoxLayout : public QHBoxLayout {
   public:
    //[[nodiscard]] is an attribute specifier in C++ that indicates that a
    //function's return value should not be ignored.
    [[nodiscard]] QSize sizeHint() const override;
};

void restartTimerForSecs(QTimer *timer, int secs);

class SliderWithLabelsLayout : public QVBoxLayout {
   public:
    SliderWithLabelsLayout();

    Wrappers::BrightnessSlider m_Slider;
    QLabel m_BrightnessLabel;
    QLabel m_DisplayNameLabel;
    std::string displayBus;
};

}  // namespace Wrappers

// When you create an object on the stack, it will be automatically
// destroyed when it goes out of scope. However, if the object is a
// member of another object (like in your case with m_Exit, m_Open, and
// m_Preferences being members of TrayMenu),it will be destroyed when
// the owning object (TrayMenu) is destroyed, and not when it goes out
// of scope.

#endif  // LUMID_WRAPPERS_HPP
