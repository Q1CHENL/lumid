//
// Created by liuqichen on 2/13/23.
//
#include <regex>
#include "MainWindow.hpp"
#include "Wrappers.hpp"
#include <QCloseEvent>
#include <QMenu>
#include <QScreen>
#include <iostream>
#include <algorithm>
#include <QMenuBar>
#include <QxtGlobalShortcut>
#include <QTimer>

using namespace Wrappers;

MainWindow::MainWindow() {
    setWindowTitle(" ");
    setFixedSize(400, 500);
    m_MainLayout.setSizeConstraint(QLayout::SetFixedSize);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    initAllLayouts();
    BrightnessSlider *mainSlider = generalSlider();
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(hide()));

    //change the path to yours
    //tray icon does not show using resource image
    trayIcon.setIcon(QIcon("/usr/share/icons/darkerbrightness-2nd-version.png"));

//    m_TrayMenu.addAction("Open", this, [=]() {
//        show();
//        restartTimerForSecs(&m_Timer, 5);
//    });
//    m_TrayMenu.addAction("Preferences", this, [=](){
//    });
//    m_TrayMenu.addAction("Exit", this, &MainWindow::onExit);
    trayIcon.setContextMenu(&m_TrayMenu);
    trayIcon.show();

    // Trigger is default for QMenu. set it to nullptr to customize
    // First click of double click is recognized as Trigger
    // I don't why this works, but it works
    connect(&trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            m_Timer.stop();
            show();
            restartTimerForSecs(&m_Timer, 10);
        }
    });

    //add layout to main
    addLayouts();

    hideButton.setTimer(&m_Timer);
    hideButton.setText("Show All");
    hideButton.setParent(this);
    connect(&hideButton, &QPushButton::clicked, this, [=]() {
        MainWindow::hideOtherSliders();
    });

    subLayoutsVex.at(0)->addWidget(&hideButton);

    auto *shortcutF5 = new QxtGlobalShortcut(QKeySequence(Qt::Key_F5), this);
    auto *shortcutF6 = new QxtGlobalShortcut(QKeySequence(Qt::Key_F6), this);

    // Connect shortcuts to slider value change signal
    // Qxt will hide keyPressEvent()
    QObject::connect(shortcutF5, &QxtGlobalShortcut::activated, mainSlider, [=]() {
        shortCutsKeyPressed(mainSlider, -10);
    });

    QObject::connect(shortcutF6, &QxtGlobalShortcut::activated, mainSlider, [=]() {
        shortCutsKeyPressed(mainSlider, 10);
    });

    installEventFilter(this);
    connect(qApp, &QApplication::focusChanged, this, &MainWindow::onFocusChanged);

    setLayout(&m_MainLayout);
}

void MainWindow::shortCutsKeyPressed(BrightnessSlider *slider, int value) {
    show();
    slider->setValue(slider->value() + value);
    restartTimerForSecs(&m_Timer, 3);// start the timer with 3 second timeout
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (trayIcon.isVisible()) {
        hide(); // hide the main window
        event->ignore(); // ignore the close event
    } else {
        event->accept(); // exit the application
    }
}

void MainWindow::onExit() {
    QCoreApplication::quit();
}

void MainWindow::initAllLayouts() {
    // <display name, bus, brightness>
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
        start_pos = result.find("Model:               ", end_pos);
        end_pos = result.find('\n', start_pos);
        std::string display_brand_name = result.substr(start_pos + 22, end_pos - start_pos - 22);
        info.emplace_back(display_brand_name, bus_number, 0);
    }
    std::sort(info.begin(), info.end(), [](std::tuple<std::string, std::string, int> a,
                                           std::tuple<std::string, std::string, int> b) {
        return std::get<1>(a) > std::get<1>(b);
    });

    displayCount = (int) info.size();
    for (int i = 0; i < displayCount; i++) {
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

    // init main layout
    // make unique directly, don't use std::unique_ptr ptr = make_unique...
    // that it will be deleted out of scope
    subLayoutsVex.emplace_back(std::make_unique<SliderWithLabelsLayout>());
    initLayout(subLayoutsVex.at(0).get(), info, 0, true);
    subLayoutsVex.at(0)->m_DisplayNameLabel.setText("General");
    connect(&(subLayoutsVex.at(0)->m_Slider), &QSlider::valueChanged, this, [=](int value) {
        for (int i = 0; i < displayCount; ++i) {
            subLayoutsVex.at(i + 1)->m_Slider.setValue(value);
        }
        subLayoutsVex.at(0)->m_BrightnessLabel.setText(QString::number(value));
    });

    //init the unique ptrs
    for (int i = 0; i < info.size(); ++i) {
        subLayoutsVex.emplace_back(std::make_unique<SliderWithLabelsLayout>());
    }

    //init except for main layout
    int j = 0;
    for (int i = 1; i < info.size() + 1; ++i, ++j) {
        initLayout(subLayoutsVex.at(i).get(), info, j, false);

        connect(&(subLayoutsVex.at(i)->m_Slider), &QSlider::valueChanged, this, [=](int value) {
            QStringList args;
            QString newValue = QString::number(value);
            QStringList arguments;
            arguments << newValue << "--async" << "--bus"
                      << QString::fromStdString(std::get<1>(info.at(j)));
            //Note that here sudo is the program, ddcutil is considered as an argument
            QProcess::startDetached("sudo", QStringList() << "ddcutil" << "setvcp" << "10" << arguments);
            subLayoutsVex.at(i)->m_BrightnessLabel.setText(QString::number(value));
        });
    }
}

void MainWindow::initLayout(SliderWithLabelsLayout *layout,
                            std::vector<std::tuple<std::string, std::string, int>> info,
                            int index,
                            bool visible
) {
    std::string displayName = std::get<0>(info.at(index));
    if (index == 0) {
        displayName += "\n(Primary)";
    }
    layout->m_DisplayNameLabel.setText(QString::fromStdString(displayName));
    layout->m_DisplayNameLabel.setAlignment(Qt::AlignCenter);
    layout->m_DisplayNameLabel.setFixedSize(100, 35);
    layout->m_DisplayNameLabel.setFrameStyle(QFrame::NoFrame);
    layout->m_DisplayNameLabel.setLineWidth(1);
    layout->m_DisplayNameLabel.setVisible(visible);

    //slider
    layout->m_Slider.setOrientation(Qt::Vertical);
    layout->m_Slider.setRange(0, 100);
    layout->m_Slider.setValue(std::get<2>(info.at(index)));
    layout->m_Slider.setSingleStep(10);
    layout->m_Slider.setTracking(false);
    layout->m_Slider.setVisible(visible);
    layout->m_Slider.setTimer(&m_Timer);

    //brightness value label
    layout->m_BrightnessLabel.setText(QString::number(std::get<2>(info.at(index))));
    setFixedSize(30, 24);
    layout->m_BrightnessLabel.setFrameStyle(QFrame::NoFrame);
    layout->m_BrightnessLabel.setLineWidth(1);
    layout->m_BrightnessLabel.setText(QString::number(std::get<2>(info.at(index))));
    layout->m_BrightnessLabel.setAlignment(Qt::AlignCenter);
    layout->m_BrightnessLabel.setVisible(visible);

    //bus
    layout->displayBus = std::get<1>(info.at(index));
}

void MainWindow::hideOtherSliders() {
    if (other_sliders_hidden) {
        performHideAndChangeButtonText("Focus",
                                       "DarkerBrightness",
                                       other_sliders_hidden,
                                       &hideButton,
                                       &m_MainLayout,
                                       &subLayoutsVex);
        return;
    }
    performHideAndChangeButtonText("Show All",
                                   " ",
                                   other_sliders_hidden,
                                   &hideButton,
                                   &m_MainLayout,
                                   &subLayoutsVex);
}

void MainWindow::performHideAndChangeButtonText(const std::string &buttonText,
                                                const std::string &windowTitle,
                                                bool currentlyHidden,
                                                QPushButton *button,
                                                SlidersHBoxLayout *layout,
                                                std::vector<std::unique_ptr<SliderWithLabelsLayout>> *vex
) {
    for (int i = 1; i < vex->size(); i++) {
        other_sliders_hidden = !currentlyHidden;
        switchVisibility(vex->at(i).get(), currentlyHidden);
    }
    layout->setSizeConstraint(QLayout::SetFixedSize);
    button->setText(QString::fromStdString(buttonText));
    setWindowTitle(QString::fromStdString(windowTitle));
}

void MainWindow::switchVisibility(SliderWithLabelsLayout *layout, bool visible) {
    layout->m_Slider.setVisible(visible);
    layout->m_BrightnessLabel.setVisible(visible);
    layout->m_DisplayNameLabel.setVisible(visible);
}

BrightnessSlider *MainWindow::generalSlider() {
    return &subLayoutsVex.at(0).get()->m_Slider;
}

void MainWindow::addLayouts() {
    for (const auto &i: subLayoutsVex) {
        m_MainLayout.addLayout(i.get());
    }
}

// restart timer when click on main window
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && (obj == this || obj == generalSlider())) {
        // Do something when the slider is clicked
        show();
        m_Timer.stop();
        m_Timer.start(5000);
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::onFocusChanged(QWidget *oldWidget, QWidget *newWidget) {
    if (newWidget == this || newWidget == generalSlider()) {
        // Do something when the main window or slider gets focus
        show();
        m_Timer.stop();
        m_Timer.start(5000);
    }
}