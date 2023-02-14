//
// Created by liuqichen on 2/13/23.
//
#include <sstream>
#include "Sliders.hpp"

int getDisplayBrightness();

Sliders::Sliders() {
    setWindowTitle("BD");
    setFixedSize(100, 250);
    int currBrightness = getDisplayBrightness();

    //value label settings
    valueLabel.setFixedSize(30, 24);
    valueLabel.setFrameStyle(QFrame::NoFrame);
    valueLabel.setLineWidth(1);
    valueLabel.setText(QString::number(currBrightness));
    valueLabel.setAlignment(Qt::AlignCenter);

    //slider 1 settings
    m_Slider_1.setOrientation(Qt::Vertical);
    m_Slider_1.setRange(0, 100);
    m_Slider_1.setValue(currBrightness);
    m_Slider_1.setSingleStep(10);

    //general layout settings
    m_Layout.addWidget(&valueLabel);
    connect(&m_Slider_1, &QSlider::valueChanged, this, &Sliders::on_value_changed);
    m_Layout.addWidget(&m_Slider_1);
    m_Layout.setAlignment(Qt::AlignHCenter);

//    m_Slider_2.setOrientation(Qt::Vertical);
//    m_Slider_2.setRange(0, 10);
//    m_Slider_2.setValue(0);
//    m_Slider_2.setSingleStep(1);
//    connect(&m_Slider_2, &QSlider::valueChanged, this, &Sliders::on_value_changed);
//    m_Layout.addWidget(&m_Slider_2);
    setLayout(&m_Layout);
}

void Sliders::on_value_changed(int value) {
    QStringList args;
    QString newValue = QString::number(value);
    QStringList arguments;

    static int last_value = 0;
    arguments << newValue << "--display";
    //Note that here sudo is the program, ddcutil is considered as an argument
    QProcess::startDetached("sudo", QStringList() << "ddcutil" << "setvcp" << "10" << arguments << "2");

    last_value = value;
    valueLabel.setText(QString::number(value));
}

// from ChatGPT
int getDisplayBrightness() {
    std::string result;
    std::string cmd = "sudo ddcutil getvcp 10 --display 2";

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
