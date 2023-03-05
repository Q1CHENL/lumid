//
// Created by liuqichen on 3/4/23.
//

#ifndef DARKERBRIGHTNESS_WRAPPERS_HPP
#define DARKERBRIGHTNESS_WRAPPERS_HPP


#include <QTimer>
#include <QSlider>
#include <QPushButton>

namespace Wrappers {

    class BrightnessSlider : public QSlider {

    public:
        QTimer *m_Timer{};

        BrightnessSlider() = default;

        void setTimer(QTimer *timer);

    };

    class ViewChangeButton : public QPushButton {

    public:
        QTimer *m_Timer{};

        ViewChangeButton() = default;

        void setTimer(QTimer *timer);
    };

    void restartTimerInSecs(QTimer *timer, int secs);
}


#endif //DARKERBRIGHTNESS_WRAPPERS_HPP
