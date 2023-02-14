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
    void on_value_changed(int value);
    QVBoxLayout m_Layout;
    QSlider m_Slider_1;
    QSlider m_Slider_2;
    QLabel valueLabel;
};

#endif //SLIDER_SLIDER_H
