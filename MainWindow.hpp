//
// Created by liuqichen on 2/13/23.
//

#ifndef SLIDER_SLIDER_H
#define SLIDER_SLIDER_H

#include <QApplication>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QProcess>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QTimer>
#include "Wrappers.hpp"
#include "SlidersHBoxLayout.hpp"
#include "SliderWithLabelsLayout.hpp"

using namespace Wrappers;

class MainWindow : public QWidget
{
public:
    MainWindow();
    static void onExit();

private:
    bool other_sliders_hidden = true;
    QSystemTrayIcon trayIcon;
    SlidersHBoxLayout m_MainLayout;
    ViewChangeButton hideButton;
    QTimer m_Timer;
    int displayCount{};

    BrightnessSlider* generalSlider();
    std::vector<std::unique_ptr<SliderWithLabelsLayout>> subLayoutsVex;
    void onFocusChanged(QWidget *oldWidget, QWidget *newWidget);
    void hideOtherSliders();
    void initAllLayouts();
    void addLayouts();
    void closeEvent(QCloseEvent *event) override;
    static void switchVisibility(SliderWithLabelsLayout* layout, bool visible);
    void initLayout(SliderWithLabelsLayout* layout, std::vector<std::tuple<std::string, std::string, int>> info, int index, bool visible);
    bool eventFilter(QObject *obj, QEvent *event) override;
    void shortCutsKeyPressed(BrightnessSlider* slider, int value);
    void performHideAndChangeButtonText(const std::string& buttonText,
                                        const std::string& windowTitle,
                                        bool currentHidden,
                                        QPushButton* button,
                                        SlidersHBoxLayout* layout,
                                        std::vector<std::unique_ptr<SliderWithLabelsLayout>>* subLayoutsVex
    );


};

#endif //SLIDER_SLIDER_H
