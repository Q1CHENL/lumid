//
// Created by liuqichen on 3/4/23.
//

#ifndef LUMID_WRAPPERS_HPP
#define LUMID_WRAPPERS_HPP

#include <QAction>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QKeySequenceEdit>

// include leads to circular dependency
class MainWindow;

namespace Wrappers {

class BrightnessSlider : public QSlider {
   public:
    QTimer *m_Timer{};

    BrightnessSlider() = default;

    void setTimer(QTimer *timer, MainWindow *mainWindow);
};

class ShowAllAndFocusButton : public QPushButton {
   public:
    QTimer *m_Timer{};

    ShowAllAndFocusButton() = default;

    void setTimer(QTimer *timer, MainWindow *mainWindow);
};

class PreferencesWindow : public QDialog {
   public:
    // todo add things
    // stride, bar, shortcuts...

    // In C++, you generally only provide default values
    // for arguments in the function declaration, not in
    // the function definition.
    PreferencesWindow(QWidget *parent);
    void closeEvent(QCloseEvent *event) override;
    int getStride() const;
    void showCentered();

   private:
    int posX, posY;

    QLabel* strideLabel;
    QSpinBox* spinbox;

    QLabel* increaseLabel;
    QLabel* decreaseLabel;
    
    // keybinding edit fields
    QKeySequenceEdit* keySeqEditIncrease;
    QKeySequenceEdit* keySeqEditDecrease;

    QPushButton* applyButton;
    QPushButton* resetButton;

    // Qt will manage their lifetime
    QHBoxLayout* strideLayout = new QHBoxLayout(); 
    QHBoxLayout* increaseLayout = new QHBoxLayout();
    QHBoxLayout* decreaseLayout = new QHBoxLayout();
    QHBoxLayout* bottomButtonLayout = new QHBoxLayout();

    QVBoxLayout* mainLayout = new QVBoxLayout();

   private slots:
    void accept() override;
    void reset();
};

class TrayMenu : public QMenu {
   public:
    TrayMenu(MainWindow *mainWindow);

    // std::unique_ptr<PreferencesWindow> m_PreferencesWindow;
    PreferencesWindow* m_PreferencesWindow;

    std::unique_ptr<QAction> m_Open;

    std::unique_ptr<QAction> m_OpenDisplaySetting;

    std::unique_ptr<QAction> m_Preferences;

    std::unique_ptr<QAction> m_Exit;

    void connectSignals(MainWindow *mainWindow);
};

class SlidersHBoxLayout : public QHBoxLayout {
   public:
    //[[nodiscard]] is an attribute specifier in C++ that indicates that a
    // function's return value should not be ignored.
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