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

Sliders::Sliders() {
    setWindowTitle("BD");
    setFixedSize(150, 250);

    info_name_bus_brightness = getDisplayInfo();
    int currBrightness_0 = std::get<2>(info_name_bus_brightness.at(0));
    int currBrightness_1 = std::get<2>(info_name_bus_brightness.at(1));

    //change the path to yours
    //tray icon does not show using resource image
    trayIcon.setIcon(QIcon("/usr/share/icons/darkerbrightness.png"));

    auto *trayMenu = new QMenu(this);
    trayMenu->addAction("Open", this, &Sliders::show);
    trayMenu->addAction("Exit", this, &Sliders::onExit);

    trayIcon.setContextMenu(trayMenu);
    trayIcon.show();

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

    //value label combine settings
    m_valueLabel_combine.setFixedSize(30, 24);
    m_valueLabel_combine.setFrameStyle(QFrame::NoFrame);
    m_valueLabel_combine.setLineWidth(1);
    m_valueLabel_combine.setText(QString::number(currBrightness_0));
    m_valueLabel_combine.setAlignment(Qt::AlignCenter);

    //slider combine settings
    m_Slider_combine.setOrientation(Qt::Vertical);
    m_Slider_combine.setRange(0, 100);
    m_Slider_combine.setValue(currBrightness_0);
    m_Slider_combine.setSingleStep(10);
    m_Slider_combine.setTracking(false);

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

    //1st layout settings
    m_Layout_combine.addWidget(&m_valueLabel_combine);
    connect(&m_Slider_combine, &QSlider::valueChanged, this, &Sliders::on_value_changed_combine);
    m_Layout_combine.addWidget(&m_Slider_combine);
    m_Layout_combine.setAlignment(Qt::AlignHCenter);

    //1st layout settings
    m_Layout_1.addWidget(&m_valueLabel_1);
    connect(&m_Slider_1, &QSlider::valueChanged, this, &Sliders::on_value_changed_0);
    m_Layout_1.addWidget(&m_Slider_1);
    m_Layout_1.setAlignment(Qt::AlignHCenter);

    m_Layout_2.addWidget(&m_valueLabel_2);
    m_Layout_2.addWidget(&m_Slider_2);
    connect(&m_Slider_2, &QSlider::valueChanged, this, &Sliders::on_value_changed_1);

    m_MainLayout.addLayout(&m_Layout_combine);
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
    arguments << newValue << "--async" << "--bus" << QString::fromStdString(std::get<1>(info_name_bus_brightness.at(0)));
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
    arguments << newValue << "--async" << "--bus" << QString::fromStdString(std::get<1>(info_name_bus_brightness.at(1)));
    //Note that here sudo is the program, ddcutil is considered as an argument
    QProcess::startDetached("sudo", QStringList() << "ddcutil" << "setvcp" << "10" << arguments);
    m_valueLabel_2.setText(QString::number(value));
}

void Sliders::on_value_changed_combine(int value) {
    m_Slider_1.setValue(value);
    m_Slider_2.setValue(value);
    m_valueLabel_combine.setText(QString::number(value));
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

//<display name, bus, brightness>
std::vector<std::tuple<std::string, std::string, int>> Sliders::getDisplayInfo() {
    std::vector<std::tuple<std::string, std::string, int>> info;

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
        //buses.emplace_back(bus_number);

        //start_pos = end_pos + 1;
        start_pos = result.find("Model:               ", end_pos);
        end_pos = result.find("\n", start_pos);
        std::string display_brand_name = result.substr(start_pos + 22, end_pos - start_pos - 22);
        info.emplace_back(display_brand_name, bus_number, 0);
    }
    std::sort(info.begin(), info.end(), [](std::tuple<std::string, std::string, int> a,
                                           std::tuple<std::string, std::string, int> b) {
        return std::get<1>(a) > std::get<1>(b);
    });

    int display_number = (int) info.size();
    for (int i = 0; i < display_number; i++) {
        std::string result_brightness;
        std::string cmd = "sudo ddcutil getvcp 10 --bus " + std::get<1>(info.at(i));
        FILE *pipe_brightness = popen(cmd.c_str(), "r");
        //if (!pipe_brightness) return -1;
        char buffer_brightness[128];
        while (!feof(pipe_brightness)) {
            if (fgets(buffer_brightness, 128, pipe_brightness) != nullptr)
                result_brightness += buffer_brightness;
        }
        pclose(pipe_brightness);
        // Extract the brightness value from the output
        int pos = (int) result_brightness.find("current value =   ") + 18;
        std::string brightnessStr = result_brightness.substr(pos, 3);
        //trim space and assign
        std::get<2>(info.at(i)) =
                std::stoi(brightnessStr);
    }

    return info;
}
