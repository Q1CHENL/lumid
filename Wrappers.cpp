//
// Created by liuqichen on 3/4/23.
//

#include "Wrappers.hpp"

#include "MainWindow.hpp"
#include "PreferencesWindow.hpp"

void Wrappers::BrightnessSlider::setTimer(QTimer* timer, MainWindow* mainWindow) {
    m_Timer = timer;
    QObject::connect(this, &QSlider::sliderReleased, this, [=]() {
        mainWindow->show();
        m_Timer->stop();
        restartTimerForSecs(timer, STAY_TIME_LONG);
    });
    QObject::connect(this, &QSlider::sliderPressed, this, [=]() {
        mainWindow->show();
        m_Timer->stop();
        restartTimerForSecs(timer, STAY_TIME_LONG);
    });
}

void Wrappers::ShowAllAndFocusButton::setTimer(QTimer* timer, MainWindow* mainWindow) {
    m_Timer = timer;
    QObject::connect(this, &QPushButton::clicked, this, [=]() {
        mainWindow->show();
        restartTimerForSecs(timer, STAY_TIME_LONG);
    });
}

void Wrappers::restartTimerForSecs(QTimer* timer, int secs) {
    timer->stop();
    timer->start(secs * 1000);
}

// The PRESSED signal is emitted when the button is first pressed down by
// the user. It is emitted immediately when the mouse button is pressed down,
// before the button is released. This signal is useful if you want to
// respond to the button being pressed down, but don't necessarily want
// to respond to the button being released or clicked.
//
// The CLICKED signal is emitted when the button is clicked by the user.
// It is emitted when the mouse button is pressed down and then released
// while over the button. This signal is useful if you want to respond to
// the button being clicked, which is the most common use case for a button.

// FROM ChatGPT:
// Yes, I'm sure about this. Using signals and slots instead of event
// filters is a general best practice when developing with Qt.
//
// Signals and slots provide a simple and intuitive way to handle
// events and communicate between different parts of your application.
// They make it easy to create loosely coupled components that can
// be easily connected and disconnected at runtime. They also make it
// easy to write testable code, since you can easily simulate events
// and verify that the correct signals are emitted and slots are called.
//
// In contrast, event filters can be more complex and difficult to
// understand, especially for developers who are new to Qt. They require
// you to subclass QObject and implement the eventFilter function,
// which can be error-prone and hard to debug. Additionally, event filters
// can be less flexible than signals and slots, since they are tightly
// coupled to the object that is being filtered.
//
// Of course, there are some cases where event filters are the best or
// only option, such as when you need to filter events across multiple
// objects or when you need to perform complex event processing that is
// not easily expressed with signals and slots. But in general, whenever
// possible, it's a good idea to use signals and slots instead of event
// filters.

QSize Wrappers::SlidersHBoxLayout::sizeHint() const {
    QSize hint = QHBoxLayout::sizeHint();
    hint.setHeight(300);  // Set a custom width for the layout
    return hint;
}

Wrappers::SliderWithLabelsLayout::SliderWithLabelsLayout() {
    addWidget(&m_BrightnessLabel, 0, Qt::AlignHCenter);
    addWidget(&m_Slider, 0, Qt::AlignHCenter);
    addWidget(&m_DisplayNameLabel, 0, Qt::AlignHCenter);
    setAlignment(Qt::AlignHCenter);
}