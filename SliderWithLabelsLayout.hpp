//
// Created by liuqichen on 2/28/23.
//

#ifndef DARKERBRIGHTNESS_SLIDERWITHLABELSLAYOUT_HPP
#define DARKERBRIGHTNESS_SLIDERWITHLABELSLAYOUT_HPP


#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>

class SliderWithLabelsLayout : public QVBoxLayout {
public:
    SliderWithLabelsLayout();

    QSlider m_Slider;
    QLabel m_BrightnessLabel;
    QLabel m_DisplayNameLabel;
    std::string displayBus;
};


#endif //DARKERBRIGHTNESS_SLIDERWITHLABELSLAYOUT_HPP
