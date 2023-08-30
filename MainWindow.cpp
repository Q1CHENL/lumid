//
// Created by liuqichen on 2/13/23.
//

#include "MainWindow.hpp"

#include <QGuiApplication>
#include <QProcess>
#include <QScreen>
#include <QxtGlobalShortcut>
#include <algorithm>
#include <regex>

#include "PreferencesWindow.hpp"
#include "Wrappers.hpp"

using namespace Wrappers;

// Initializing a member variable like TrayMenu m_TrayMenu(this); directly within
// the class definition usually works for types with default constructors. However,
// you're trying to pass this pointer as an argument to TrayMenu, which requires
// the MainWindow object to be fully constructed. But m_TrayMenu is also a part of
// the MainWindow object, so you're essentially passing an incomplete object to the
// constructor of m_TrayMenu.
MainWindow::MainWindow() {
    setWindowTitle(" ");
    setFixedSize(MAIN_WINDOW_X, MAIN_WINDOW_Y);
    m_MainLayout.setSizeConstraint(QLayout::SetFixedSize);

    /**
     * Display geometry in Qt:
     *             ______
     *  _________ |      |
     * |         ||      |
     * |    0    ||  1   |
     * |_________||      |
     *            |______|
     * Assume we have 2x 1080 * 1920 displays arranged as above, the whole
     * display area qt recognizes is actually (1920 + 1080) * 1920.
     *
     * So for the primary display (screens.at(0)):
     * geometry.top() == e.g 466, not 0
     * geometry.bottom() == e.g 1545, not 1080
     *
     */
    // App appears at position posX, posY: top left
    QList<QScreen *> screens = QGuiApplication::screens();
    QRect primaryGeometry = screens.at(0)->geometry();  // primary display's geometry
    posX = primaryGeometry.left() + POS_FROM_LEFT_EDGE;
    posY = primaryGeometry.top() + (primaryGeometry.bottom() - primaryGeometry.top()) * (1 - VERTICAL_POS_FACTOR);

    // m_PreferencesWindow = new PreferencesWindow();

    initAllLayouts();

    // BrightnessSlider *mainSlider = generalSlider();
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(hide()));

    // Add layouts to main
    addLayouts();

    hideButton.setTimer(&m_Timer, this);
    hideButton.setText("Show All");
    hideButton.setParent(this);
    connect(&hideButton, &QPushButton::clicked, this,
            [=]() { MainWindow::hideOtherSliders(); });

    sliderLayouts.at(0)->addWidget(&hideButton);

    installEventFilter(this);

    setLayout(&m_MainLayout);

    // default keybindings
    // Qxt will hide keyPressEvent()
    bindShortcut(
        increaseShortcut.get(),
        decreaseShortcut.get());

    this->setWindowFlags(
        Qt::Window |               // make the widget a window
        Qt::FramelessWindowHint |  // disable title bar and buttons on it
        Qt::Tool |                 // prevent showing in dock
        // Qt::WindowDoesNotAcceptFocus |  // prevent dock showing up but the window is not on top
        Qt::WindowStaysOnTopHint);  // make the window on top layer
}

void MainWindow::shortCutsKeyPressed(BrightnessSlider *slider, int stride) {
    if (this->isVisible()) {
        show();
    } else {
        showOnTopLeft();
    }
    this->lower();  // prevent dock showing up
    slider->setValue(slider->value() + stride);
    restartTimerForSecs(&m_Timer, STAY_TIME_SHORT);  // Start the timer with 3 second timeout
}

void MainWindow::onExit() { QCoreApplication::quit(); }

void MainWindow::initAllLayouts() {
    // Run the ddcutil command to get the display info
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("ddcutil detect", "r"),
                                                  pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to run ddcutil command");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
           nullptr) {
        result += buffer.data();
    }
    // Parse the output of the ddcutil command to get the bus numbers
    std::size_t start_pos = 0;
    while ((start_pos = result.find("I2C bus:", start_pos)) !=
           std::string::npos) {
        // Get the display name
        std::size_t end_pos = result.find('\n', start_pos);
        std::string bus_number =
            result.substr(start_pos + 19, end_pos - start_pos - 19);
        start_pos = result.find("Model:               ", end_pos);
        end_pos = result.find('\n', start_pos);
        std::string display_brand_name =
            result.substr(start_pos + 22, end_pos - start_pos - 22);
        info.emplace_back(display_brand_name, bus_number, 0);
    }
    std::sort(info.begin(), info.end(),
              [](std::tuple<std::string, std::string, int> a,
                 std::tuple<std::string, std::string, int> b) {
                  return std::get<1>(a) > std::get<1>(b);
              });

    displayCount = (int)info.size();
    for (int i = 0; i < displayCount; i++) {
        std::string result_brightness;
        std::string cmd =
            "sudo ddcutil getvcp 10 --bus " + std::get<1>(info.at(i));
        FILE *pipe_brightness = popen(cmd.c_str(), "r");
        // if (!pipe_brightness) return -1;
        char buffer_brightness[128];
        while (!feof(pipe_brightness)) {
            if (fgets(buffer_brightness, 128, pipe_brightness) != nullptr)
                result_brightness += buffer_brightness;
        }
        pclose(pipe_brightness);
        // Extract the brightness value from the output
        int pos = (int)result_brightness.find("current value =   ") + 18;
        std::string brightnessStr = result_brightness.substr(pos, 3);
        // trim space and assign
        std::get<2>(info.at(i)) = std::stoi(brightnessStr);
    }

    // init main layout
    // make unique directly, don't use std::unique_ptr ptr = make_unique...
    // that it will be deleted out of scope
    sliderLayouts.emplace_back(std::make_unique<SliderWithLabelsLayout>());
    initLayout(sliderLayouts.at(0).get(), info, 0, true);
    sliderLayouts.at(0)->m_DisplayNameLabel.setText("General");

    // init the timer for main slider
    generalSliderTimer = std::make_unique<QTimer>();
    generalSliderTimer->setSingleShot(true);

    connect(generalSliderTimer.get(), &QTimer::timeout, this, [&]() {
        int value = sliderLayouts.at(0)->m_Slider.value();
        for (int i = 0; i < displayCount; ++i) {
            sliderLayouts.at(i + 1)->m_Slider.setValue(value);
        }
        sliderLayouts.at(0)->m_BrightnessLabel.setText(QString::number(value));
    });

    connect(&(sliderLayouts.at(0)->m_Slider), &QSlider::valueChanged, this,
            [=](int value) {
                generalSliderTimer->start(300);
                sliderLayouts.at(0)->m_BrightnessLabel.setText(
                    QString::number(value));
            });

    // init the unique ptrs
    for (int i = 0; i < info.size(); ++i) {
        sliderLayouts.emplace_back(std::make_unique<SliderWithLabelsLayout>());
    }

    // init except for main layout
    int j = 0;
    for (int i = 1; i < info.size() + 1; ++i, ++j) {
        initLayout(sliderLayouts.at(i).get(), info, j, false);

        connect(&(sliderLayouts.at(i)->m_Slider), &QSlider::valueChanged, this,
                [=](int value) {
                    QStringList args;
                    QString newValue = QString::number(value);
                    QStringList arguments;

                    // https://www.ddcutil.com/faq/
                    // Option --sleep-multiplier. This option adjusts the length
                    // of time ddcutil spends in DDC/CI mandated waits. For
                    // example, if the DDC/CI protocol specifies a 40 ms wait
                    // between the time a command is sent to the monitor and the
                    // time a reply is read, and --sleep-multiplier .2 is given,
                    // ddcutil will only wait (.2 x 40 ms) = 8 ms. Some monitors
                    // have been found to communicate successfully with
                    // --sleep-multiplier values as low as .1. On the other
                    // hand, some monitors with poor DDC/CI implementations
                    // perform better if the sleep time is increased
                    //  by using a value greater than 1.

                    // .03 is generally a relatively safe multiplier value
                    arguments << newValue << "--async"
                              << "--bus"
                              << QString::fromStdString(std::get<1>(info.at(j)))
                              << "--sleep-multiplier" << QString::number(SLEEP_MULTIPLIER);
                    // Note that here sudo is the program, ddcutil is considered
                    // as an argument
                    QProcess::startDetached("sudo", QStringList()
                                                        << "ddcutil"
                                                        << "setvcp"
                                                        << "10" << arguments);
                    sliderLayouts.at(i)->m_BrightnessLabel.setText(
                        QString::number(value));
                });
    }
}

void MainWindow::initLayout(
    SliderWithLabelsLayout *layout,
    std::vector<std::tuple<std::string, std::string, int>> info, int index,
    bool visible) {
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

    // Slider
    layout->m_Slider.setOrientation(Qt::Vertical);
    layout->m_Slider.setRange(0, 100);
    layout->m_Slider.setValue(std::get<2>(info.at(index)));
    layout->m_Slider.setSingleStep(10);
    layout->m_Slider.setTracking(false);
    layout->m_Slider.setVisible(visible);
    layout->m_Slider.setTimer(&m_Timer, this);

    // Brightness value label
    layout->m_BrightnessLabel.setText(
        QString::number(std::get<2>(info.at(index))));
    setFixedSize(30, 24);
    layout->m_BrightnessLabel.setFrameStyle(QFrame::NoFrame);
    layout->m_BrightnessLabel.setLineWidth(1);
    layout->m_BrightnessLabel.setText(
        QString::number(std::get<2>(info.at(index))));
    layout->m_BrightnessLabel.setAlignment(Qt::AlignCenter);
    layout->m_BrightnessLabel.setVisible(visible);

    // Bus
    layout->displayBus = std::get<1>(info.at(index));
}

void MainWindow::hideOtherSliders() {
    if (other_sliders_hidden) {
        performHideAndChangeButtonText("Focus", "Lumid", other_sliders_hidden,
                                       &hideButton, &m_MainLayout,
                                       &sliderLayouts);
        return;
    }
    performHideAndChangeButtonText("Show All", " ", other_sliders_hidden,
                                   &hideButton, &m_MainLayout, &sliderLayouts);
}

void MainWindow::performHideAndChangeButtonText(
    const std::string &buttonText, const std::string &windowTitle,
    bool currentlyHidden, QPushButton *button, SlidersHBoxLayout *layout,
    std::vector<std::unique_ptr<SliderWithLabelsLayout>> *vex) {
    for (int i = 1; i < vex->size(); i++) {
        other_sliders_hidden = !currentlyHidden;
        switchVisibility(vex->at(i).get(), currentlyHidden);
    }
    layout->setSizeConstraint(QLayout::SetFixedSize);
    button->setText(QString::fromStdString(buttonText));
    setWindowTitle(QString::fromStdString(windowTitle));
}

void MainWindow::switchVisibility(SliderWithLabelsLayout *layout,
                                  bool visible) {
    layout->m_Slider.setVisible(visible);
    layout->m_BrightnessLabel.setVisible(visible);
    layout->m_DisplayNameLabel.setVisible(visible);
}

BrightnessSlider *MainWindow::generalSlider() {
    return &sliderLayouts.at(0).get()->m_Slider;
}

void MainWindow::addLayouts() {
    for (const auto &i : sliderLayouts) {
        m_MainLayout.addLayout(i.get());
    }
}

// Restart timer when main window clicked
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress &&
        (obj == this || obj == generalSlider())) {
        // Do something when the slider is clicked
        show();
        restartTimerForSecs(&m_Timer, STAY_TIME_LONG);
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::showOnTopLeft() {
    move(posX, posY);
    show();
    raise();
    restartTimerForSecs(&m_Timer, STAY_TIME_LONG);
}

void MainWindow::setStride(int stride) {
    this->stride = stride;
}

void MainWindow::setShortcuts(QKeySequence *increase, QKeySequence *decrease) {
    this->increaseShortcut->setShortcut(*increase);
    this->decreaseShortcut->setShortcut(*decrease);
}

void MainWindow::bindShortcut(QxtGlobalShortcut *increase, QxtGlobalShortcut *decrease) {
    BrightnessSlider *mainSlider = generalSlider();
    QObject::connect(increase, &QxtGlobalShortcut::activated, mainSlider,
                     [=]() { shortCutsKeyPressed(mainSlider, stride); });
    QObject::connect(decrease, &QxtGlobalShortcut::activated, mainSlider,
                     [=]() { shortCutsKeyPressed(mainSlider, -stride); });
}