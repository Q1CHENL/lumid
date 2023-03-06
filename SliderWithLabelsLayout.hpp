//
// Created by liuqichen on 2/28/23.
//

#ifndef LUMID_SLIDERWITHLABELSLAYOUT_HPP
#define LUMID_SLIDERWITHLABELSLAYOUT_HPP


#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include "Wrappers.hpp"


class SliderWithLabelsLayout : public QVBoxLayout {
public:
    SliderWithLabelsLayout();

    Wrappers::BrightnessSlider m_Slider;
    QLabel m_BrightnessLabel;
    QLabel m_DisplayNameLabel;
    std::string displayBus;
};


#endif //LUMID_SLIDERWITHLABELSLAYOUT_HPP
