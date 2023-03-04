//
// Created by liuqichen on 3/4/23.
//

#include "BrightnessSlider.hpp"

void BrightnessSlider::setTimer(QTimer *timer) {
    m_Timer = timer;
    QObject::connect(this, &QSlider::sliderReleased, this, [=]() {
        show();
        m_Timer->stop();
        m_Timer->start(5000);
    });
    QObject::connect(this, &QSlider::sliderPressed, this, [=]() {
        show();
        m_Timer->stop();
        m_Timer->start(5000);
    });
}

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


