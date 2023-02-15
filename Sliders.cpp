//
// Created by liuqichen on 2/13/23.
//
#include <sstream>
#include "Sliders.hpp"

Sliders::Sliders() {
    setWindowTitle("BD");
    setFixedSize(100, 250);
    int currBrightness_1 = getDisplayBrightness(3);
    int currBrightness_2 = getDisplayBrightness(2);

    //value label 1 settings
    m_valueLabel_1.setFixedSize(30, 24);
    m_valueLabel_1.setFrameStyle(QFrame::NoFrame);
    m_valueLabel_1.setLineWidth(1);
    m_valueLabel_1.setText(QString::number(currBrightness_1));
    m_valueLabel_1.setAlignment(Qt::AlignCenter);

    //value label 2 settings
    m_valueLabel_2.setFixedSize(30, 24);
    m_valueLabel_2.setFrameStyle(QFrame::NoFrame);
    m_valueLabel_2.setLineWidth(1);
    m_valueLabel_2.setText(QString::number(currBrightness_2));
    m_valueLabel_2.setAlignment(Qt::AlignCenter);

    //slider 1 settings
    m_Slider_1.setOrientation(Qt::Vertical);
    m_Slider_1.setRange(0, 100);
    m_Slider_1.setValue(currBrightness_1);
    m_Slider_1.setSingleStep(10);
    m_Slider_1.setTracking(false);

    //Slider 2 settings
    m_Slider_2.setOrientation(Qt::Vertical);
    m_Slider_2.setRange(0, 100);
    m_Slider_2.setValue(currBrightness_2);
    m_Slider_2.setSingleStep(10);
    m_Slider_2.setTracking(false);

    //general layout settings
    m_Layout_1.addWidget(&m_valueLabel_1);
    connect(&m_Slider_1, &QSlider::valueChanged, this, &Sliders::on_value_changed_1);
    m_Layout_1.addWidget(&m_Slider_1);
    m_Layout_1.setAlignment(Qt::AlignHCenter);

    m_Layout_2.addWidget(&m_valueLabel_2);
    m_Layout_2.addWidget(&m_Slider_2);
    connect(&m_Slider_2, &QSlider::valueChanged, this, &Sliders::on_value_changed_2);

    m_MainLayout.addLayout(&m_Layout_1);
    m_MainLayout.addLayout(&m_Layout_2);
    setLayout(&m_MainLayout);

}

void Sliders::on_value_changed_1(int value) {
    QStringList args;
    QString newValue = QString::number(value);
    QStringList arguments;
    //change the bus number after --bus to your monitors'
    //use sudo cat /dev/i2c* to list all buses
    arguments << newValue << "--async" << "--bus" << "3";
    //Note that here sudo is the program, ddcutil is considered as an argument
    QProcess::startDetached("sudo", QStringList() << "ddcutil" << "setvcp" << "10" << arguments);
    m_valueLabel_1.setText(QString::number(value));
}


void Sliders::on_value_changed_2(int value) {
    QStringList args;
    QString newValue = QString::number(value);
    QStringList arguments;
    //change the bus number after --bus to your monitors'
    //use sudo cat /dev/i2c* to list all buses
    arguments << newValue << "--async" << "--bus" << "2";
    //Note that here sudo is the program, ddcutil is considered as an argument
    QProcess::startDetached("sudo", QStringList() << "ddcutil" << "setvcp" << "10" << arguments);
    m_valueLabel_2.setText(QString::number(value));
}

// from ChatGPT
int Sliders::getDisplayBrightness(int display_bus) {
    std::string result;
    std::string cmd = "sudo ddcutil getvcp 10 --bus " + std::to_string(display_bus);

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return -1;

    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }
    pclose(pipe);

    // Extract the brightness value from the output
    int pos = (int) result.find("current value =   ") + 17;
    std::string brightnessStr = result.substr(pos, 4);
    int brightness = 0;
    std::istringstream(brightnessStr) >> brightness;

    return std::stoi(brightnessStr);
}
