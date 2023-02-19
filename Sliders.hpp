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

class Sliders : public QWidget
{
public:
    Sliders();
    static void onExit();

private:
    bool other_sliders_hidden = true;
    QSystemTrayIcon trayIcon;
    QVBoxLayout m_Layout_combine;
    QVBoxLayout m_Layout_1;
    QVBoxLayout m_Layout_2;
    SlidersHBoxLayout m_MainLayout;
    QSlider m_Slider_combine;
    QSlider m_Slider_1;
    QSlider m_Slider_2;
    QLabel m_valueLabel_combine;
    QLabel m_valueLabel_1;
    QLabel m_valueLabel_2;
    QPushButton hideButton;
    std::vector<std::tuple<std::string, std::string, int>> info_name_bus_brightness;
    QTimer m_timer;

    void hideOtherSliders(QLabel* display_0, QLabel* display_1);
    void on_value_changed_0(int value);
    void on_value_changed_1(int value);
    void on_value_changed_combine(int value);
    static std::vector<std::tuple<std::string, std::string, int>> getDisplayInfo();
    void closeEvent(QCloseEvent *event) override;
};

#endif //SLIDER_SLIDER_H
