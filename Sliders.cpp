//
// Created by liuqichen on 2/13/23.
//
#include <regex>
#include "Sliders.hpp"
#include <QCloseEvent>
#include <QMenu>
#include <QDebug>
#include <QScreen>
#include <iostream>
#include <algorithm>

int getNumDisplays();
std::vector<std::string> getDisplayInfo();

Sliders::Sliders() {
    setWindowTitle("BD");
    setFixedSize(100, 250);
    buses = getDisplayInfo();

    //change the path to yours
    //tray icon does not show using resource image
    trayIcon.setIcon(QIcon("/usr/share/icons/darkerbrightness.png"));

    auto *trayMenu = new QMenu(this);
    trayMenu->addAction("Open", this, &Sliders::show);
    trayMenu->addAction("Exit", this, &Sliders::onExit);

    trayIcon.setContextMenu(trayMenu);
    trayIcon.show();

    int currBrightness_0 = getDisplayBrightness(std::stoi(buses.at(0)));
    int currBrightness_1 = getDisplayBrightness(std::stoi(buses.at(1)));

    //value label 1 settings
    m_valueLabel_1.setFixedSize(30, 24);
    m_valueLabel_1.setFrameStyle(QFrame::NoFrame);
    m_valueLabel_1.setLineWidth(1);
    m_valueLabel_1.setText(QString::number(currBrightness_0));
    m_valueLabel_1.setAlignment(Qt::AlignCenter);

    //value label 2 settings
    m_valueLabel_2.setFixedSize(30, 24);
    m_valueLabel_2.setFrameStyle(QFrame::NoFrame);
    m_valueLabel_2.setLineWidth(1);
    m_valueLabel_2.setText(QString::number(currBrightness_1));
    m_valueLabel_2.setAlignment(Qt::AlignCenter);

    //slider 1 settings
    m_Slider_1.setOrientation(Qt::Vertical);
    m_Slider_1.setRange(0, 100);
    m_Slider_1.setValue(currBrightness_0);
    m_Slider_1.setSingleStep(10);
    m_Slider_1.setTracking(false);

    //Slider 2 settings
    m_Slider_2.setOrientation(Qt::Vertical);
    m_Slider_2.setRange(0, 100);
    m_Slider_2.setValue(currBrightness_1);
    m_Slider_2.setSingleStep(10);
    m_Slider_2.setTracking(false);

    //general layout settings
    m_Layout_1.addWidget(&m_valueLabel_1);
    connect(&m_Slider_1, &QSlider::valueChanged, this, &Sliders::on_value_changed_0);
    m_Layout_1.addWidget(&m_Slider_1);
    m_Layout_1.setAlignment(Qt::AlignHCenter);

    m_Layout_2.addWidget(&m_valueLabel_2);
    m_Layout_2.addWidget(&m_Slider_2);
    connect(&m_Slider_2, &QSlider::valueChanged, this, &Sliders::on_value_changed_1);

    m_MainLayout.addLayout(&m_Layout_1);
    m_MainLayout.addLayout(&m_Layout_2);
    setLayout(&m_MainLayout);

}

void Sliders::on_value_changed_0(int value) {
    QStringList args;
    QString newValue = QString::number(value);
    QStringList arguments;
    //change the bus number after --bus to your monitors'
    //use sudo cat /dev/i2c* to list all buses
    arguments << newValue << "--async" << "--bus" << QString::number(std::stoi(buses.at(0)));
    //Note that here sudo is the program, ddcutil is considered as an argument
    QProcess::startDetached("sudo", QStringList() << "ddcutil" << "setvcp" << "10" << arguments);
    m_valueLabel_1.setText(QString::number(value));
}


void Sliders::on_value_changed_1(int value) {
    QStringList args;
    QString newValue = QString::number(value);
    QStringList arguments;
    //change the bus number after --bus to your monitors'
    //use sudo cat /dev/i2c* to list all buses
    arguments << newValue << "--async" << "--bus" << QString::number(std::stoi(buses.at(1)));
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

void Sliders::closeEvent(QCloseEvent *event) {
    if (trayIcon.isVisible()) {
        hide(); // hide the main window
        event->ignore(); // ignore the close event
    } else {
        event->accept(); // exit the application
    }
}

void Sliders::onExit() {
    QCoreApplication::quit();
}

int getNumDisplays() {
    return QGuiApplication::screens().count();
}

std::vector<std::string> getDisplayInfo() {
    std::vector<std::string> buses;
    // Run the ddcutil command to get the display info
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("ddcutil detect", "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to run ddcutil command");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    // Parse the output of the ddcutil command to get the bus numbers
    std::size_t start_pos = 0;
    while ((start_pos = result.find("I2C bus:", start_pos)) != std::string::npos) {
        // Get the display name
        std::size_t end_pos = result.find('\n', start_pos);
        std::string bus_number = result.substr(start_pos + 19, end_pos - start_pos - 19);
        start_pos = end_pos + 1;
        buses.emplace_back(bus_number);
    }
    std::sort(buses.begin(), buses.end(), [](std::string a, std::string b){return a > b;});
    return buses;
}
