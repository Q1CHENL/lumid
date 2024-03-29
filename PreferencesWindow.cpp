#include "PreferencesWindow.hpp"

#include <QCloseEvent>
#include <QGuiApplication>
#include <QScreen>

#include "MainWindow.hpp"

PreferencesWindow::PreferencesWindow() : QDialog() {
    setWindowTitle("Preferences");
    setFixedSize(PREFSWINDOW_WIDTH, PREFSWINDOW_HEIGHT);

    strideLayout = new QHBoxLayout();
    increaseLayout = new QHBoxLayout();
    decreaseLayout = new QHBoxLayout();
    bottomButtonLayout = new QHBoxLayout();
    mainLayout = new QVBoxLayout();

    strideLabel = new QLabel(" Stride:  ");
    strideLayout->addWidget(strideLabel);

    spinbox = new QSpinBox();  // Qt will manage its lifetime
    spinbox->setMinimum(1);
    spinbox->setMaximum(100);
    spinbox->setValue(10);
    spinbox->setFixedWidth(LABEL_SPINBOX_WIDTH);

    keySeqEditIncrease = new QKeySequenceEdit(Qt::Key_F6);
    keySeqEditDecrease = new QKeySequenceEdit(Qt::Key_F5);

    keySeqEditIncrease->setFixedWidth(LABEL_SPINBOX_WIDTH);
    keySeqEditDecrease->setFixedWidth(LABEL_SPINBOX_WIDTH);

    increaseLabel = new QLabel(" Increase:");
    increaseLayout->addWidget(increaseLabel);
    increaseLayout->addWidget(keySeqEditIncrease);

    decreaseLabel = new QLabel(" Decrease:");
    decreaseLayout->addWidget(decreaseLabel);
    decreaseLayout->addWidget(keySeqEditDecrease);

    resetButton = new QPushButton("Reset", this);
    applyButton = new QPushButton("Apply", this);

    bottomButtonLayout->addWidget(resetButton);
    bottomButtonLayout->addWidget(applyButton);

    strideLayout->addWidget(spinbox);
    mainLayout->addLayout(strideLayout);
    mainLayout->addLayout(increaseLayout);
    mainLayout->addLayout(decreaseLayout);
    mainLayout->addLayout(bottomButtonLayout);
    setLayout(mainLayout);

    // Make preferences appear in the center
    QRect screenGeometry = QGuiApplication::screens().at(0)->geometry();
    posX = ((screenGeometry.width() - this->width()) / 2) - this->width() / 2;
    posY = (screenGeometry.height() - this->height()) / 2 +
           screenGeometry.top() - this->height() / 2;
}

void PreferencesWindow::closeEvent(QCloseEvent* event) {
    event->ignore();
    hide();
}

void PreferencesWindow::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);

    QRect screenGeometry = QGuiApplication::screens().at(0)->geometry();
    posX = ((screenGeometry.width() - this->width()) / 2);
    posY = (screenGeometry.height() - this->height()) / 2 + screenGeometry.top();
    this->move(posX, posY);
}
