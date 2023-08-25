//
// Created by liuqichen on 2/13/23.
//

#ifndef SLIDER_SLIDER_H
#define SLIDER_SLIDER_H

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

#include "Wrappers.hpp"

#define POS_FROM_LEFT_EDGE 50
#define VERTICAL_POS_FACTOR 0.618
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

    static void onExit();

    void showOnTopLeft();

   private:
    int posX;
    int posY;
    bool other_sliders_hidden = true;
    QSystemTrayIcon trayIcon;
    SlidersHBoxLayout m_MainLayout;
    ViewChangeButton hideButton;
    QTimer m_Timer;
    QTimer click_tmr;
    TrayMenu m_TrayMenu;
    int displayCount{};

    BrightnessSlider *generalSlider();

    std::vector<std::unique_ptr<SliderWithLabelsLayout>> subLayoutsVex;

    std::vector<std::unique_ptr<QTimer>> slider_delay_timers;

    std::vector<std::tuple<std::string, std::string, int>> info;

    void hideOtherSliders();

    void initAllLayouts();

    void addLayouts();

    void closeEvent(QCloseEvent *event) override;

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