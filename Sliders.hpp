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

class Sliders : public QWidget
{
public:
    Sliders();

private:
    void on_value_changed_1(int value);
    void on_value_changed_2(int value);
    static int getDisplayBrightness(int display_bus);
    QVBoxLayout m_Layout_1;
    QVBoxLayout m_Layout_2;
    QHBoxLayout m_MainLayout;
    QSlider m_Slider_1;
    QSlider m_Slider_2;
    QLabel m_valueLabel_1;
    QLabel m_valueLabel_2;
};

#endif //SLIDER_SLIDER_H
