//
// Created by liuqichen on 2/13/23.
//

#include "Slider.h"

Slider::Slider(){
    setWindowTitle("Slider");
    setFixedSize(100, 200);
    m_Slider.setOrientation(Qt::Vertical);
    m_Slider.setRange(0, 10);
    m_Slider.setValue(0);
    m_Slider.setSingleStep(1);
    connect(&m_Slider, &QSlider::valueChanged, this, &Slider::on_value_changed);
    m_Layout.addWidget(&m_Slider);
    setLayout(&m_Layout);
}

void Slider::on_value_changed(int value) {
    QStringList args;
    args.append("2");
    static int last_value = 0;
    if (value > last_value) {
        QProcess::startDetached("bu", args);
    } else if (value < last_value) {
        QProcess::startDetached("bd", args);
    }
    last_value = value;
}
