//
// Created by liuqichen on 2/28/23.
//

#include "SliderWithLabelsLayout.hpp"

SliderWithLabelsLayout::SliderWithLabelsLayout() {
    addWidget(&m_BrightnessLabel,  0, Qt::AlignHCenter);
    addWidget(&m_Slider, 0, Qt::AlignHCenter);
    addWidget(&m_DisplayNameLabel, 0, Qt::AlignHCenter);
    setAlignment(Qt::AlignHCenter);
}
