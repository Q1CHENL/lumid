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
#include "SlidersHBoxLayout.hpp"
#include "SliderWithLabelsLayout.hpp"

class MainWindow : public QWidget
{
public:
    MainWindow();
    static void onExit();

private:
    bool other_sliders_hidden = true;
    QSystemTrayIcon trayIcon;
    SlidersHBoxLayout m_MainLayout;
    QPushButton hideButton;
    QTimer m_Timer;

    std::vector<std::unique_ptr<SliderWithLabelsLayout>> subLayoutsVex;

    void hideOtherSliders();
    void initAllLayouts();
    void addLayouts();
    void closeEvent(QCloseEvent *event) override;
    QSlider* generalSlider();
    static void switchVisibility(SliderWithLabelsLayout* layout, bool visible);
    void initLayout(SliderWithLabelsLayout* layout, std::vector<std::tuple<std::string, std::string, int>> info, int index, bool visible);
};

#endif //SLIDER_SLIDER_H
