//
// Created by liuqichen on 3/4/23.
//

#ifndef DARKERBRIGHTNESS_BRIGHTNESSSLIDER_HPP
#define DARKERBRIGHTNESS_BRIGHTNESSSLIDER_HPP


#include <QTimer>
#include <QSlider>

class BrightnessSlider : public QSlider{

public:
    QTimer* m_Timer{};
    BrightnessSlider() = default;
    void setTimer(QTimer* timer);

};


#endif //DARKERBRIGHTNESS_BRIGHTNESSSLIDER_HPP
