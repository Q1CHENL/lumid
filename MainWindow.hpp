//
// Created by liuqichen on 2/13/23.
//

#ifndef LUMID_MAINWINDOW_H
#define LUMID_MAINWINDOW_H

#include <QApplication>
#include <QLabel>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QxtGlobalShortcut>

#include "Wrappers.hpp"

#define POS_FROM_LEFT_EDGE 50
#define VERTICAL_POS_FACTOR 0.75
#define SLEEP_MULTIPLIER .03
#define STAY_TIME_LONG 6.5
#define STAY_TIME_SHORT 3
#define STRIDE 10
#define MAIN_WINDOW_X 400
#define MAIN_WINDOW_Y 500

using namespace Wrappers;

class MainWindow : public QWidget {
   public:
    MainWindow();
   
    QTimer m_Timer;

    PreferencesWindow *m_PreferencesWindow;

    static void onExit();

    void showOnTopLeft();

    void setStride(int stride);

    // todo make pass by ptr or ref
    void setShortcuts(QKeySequence *increase, QKeySequence *decrease);

    void bindShortcut(QxtGlobalShortcut *increase, QxtGlobalShortcut *decrease);

   private:
    Q_OBJECT

    int posX;
    int posY;
    bool other_sliders_hidden = true;
    int stride = 10;
   //  QSystemTrayIcon trayIcon;
    SlidersHBoxLayout m_MainLayout;
    ShowAllAndFocusButton hideButton;

    QTimer click_tmr;
   //  TrayMenu m_TrayMenu;

    std::unique_ptr<QxtGlobalShortcut> increaseShortcut = std::make_unique<QxtGlobalShortcut>(Qt::Key_F6);
    std::unique_ptr<QxtGlobalShortcut> decreaseShortcut = std::make_unique<QxtGlobalShortcut>(Qt::Key_F5);

    int displayCount{};

    BrightnessSlider *generalSlider();

    std::vector<std::unique_ptr<SliderWithLabelsLayout>> subLayoutsVex;

    std::unique_ptr<QTimer> generalSliderTimer;

    std::vector<std::tuple<std::string, std::string, int>> info;

    void hideOtherSliders();

    void initAllLayouts();

    void addLayouts();

   //  void closeEvent(QCloseEvent *event) override;

    static void switchVisibility(SliderWithLabelsLayout *layout, bool visible);

    void
    initLayout(SliderWithLabelsLayout *layout, std::vector<std::tuple<std::string, std::string, int>> info, int index,
               bool visible);

    bool eventFilter(QObject *obj, QEvent *event) override;

    void shortCutsKeyPressed(BrightnessSlider *slider, int stride);

    void performHideAndChangeButtonText(const std::string &buttonText,
                                        const std::string &windowTitle,
                                        bool currentHidden,
                                        QPushButton *button,
                                        SlidersHBoxLayout *layout,
                                        std::vector<std::unique_ptr<SliderWithLabelsLayout>> *subLayoutsVex);
};

#endif  // SLIDER_SLIDER_H