#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QKeySequenceEdit>
#include <QGuiApplication>
#include <QCloseEvent>
#include <QScreen>

#include "PreferencesWindow.hpp"
#include "MainWindow.hpp"


PreferencesWindow::PreferencesWindow(QWidget* parent) : QDialog(parent, Qt::WindowStaysOnTopHint) {
    setWindowTitle("Preferences");

    strideLayout = new QHBoxLayout(); 
    increaseLayout = new QHBoxLayout(); 
    decreaseLayout = new QHBoxLayout();
    bottomButtonLayout = new QHBoxLayout();
    mainLayout = new QVBoxLayout();

    // Make preferences appear in the center
    QRect screenGeometry = QGuiApplication::screens().at(0)->geometry();
    posX = ((screenGeometry.width() - this->width()) / 2) - this->width() / 2;
    posY = (screenGeometry.height() - this->height()) / 2 + screenGeometry.top() - this->height() / 2;

    strideLabel = new QLabel("Stride: ");
    strideLayout->addWidget(strideLabel);
    
    spinbox = new QSpinBox(); // Qt will manage its lifetime
    spinbox->setMinimum(1);
    spinbox->setMaximum(100);
    spinbox->setValue(10);

    keySeqEditIncrease = new QKeySequenceEdit(Qt::Key_F6);
    keySeqEditDecrease = new QKeySequenceEdit(Qt::Key_F5);

    increaseLabel = new QLabel("Increase: ");
    increaseLayout->addWidget(increaseLabel);
    increaseLayout->addWidget(keySeqEditIncrease);

    decreaseLabel = new QLabel("Decrease: ");
    decreaseLayout->addWidget(decreaseLabel);
    decreaseLayout->addWidget(keySeqEditDecrease);

    resetButton = new QPushButton("Reset", this);
    connect(resetButton, &QPushButton::clicked, this, &PreferencesWindow::reset);

    applyButton = new QPushButton("Apply", this); // Qt will manage its lifetime
    connect(applyButton, &QPushButton::clicked, this, &PreferencesWindow::accept);
    
    bottomButtonLayout->addWidget(resetButton);
    bottomButtonLayout->addWidget(applyButton);

    strideLayout->addWidget(spinbox);
    mainLayout->addLayout(strideLayout);
    mainLayout->addLayout(increaseLayout);
    mainLayout->addLayout(decreaseLayout);
    mainLayout->addLayout(bottomButtonLayout);
    setLayout(mainLayout);
}

void PreferencesWindow::closeEvent(QCloseEvent* event) {
    event->ignore();
    hide();
}

void PreferencesWindow::accept() {
    MainWindow* mw = qobject_cast<MainWindow*>(this->parent());
    mw->setStride(spinbox->value());

    std::unique_ptr<QKeySequence> increaseSeq = std::make_unique<QKeySequence>(keySeqEditIncrease->keySequence());
    std::unique_ptr<QKeySequence> decreaseSeq = std::make_unique<QKeySequence>(keySeqEditDecrease->keySequence());

    mw->setShortcuts(
        std::make_unique<QKeySequence>(keySeqEditIncrease->keySequence()).get(), 
        std::make_unique<QKeySequence>(keySeqEditDecrease->keySequence()).get());
    
    // don't know why this makes the program quit, maybe because qt thinks that the prefs is the last window
    // QDialog::accept(); 
    this->hide();
}

void PreferencesWindow::reset() {
    // reset stride
    spinbox->setValue(10);
    MainWindow* mw = qobject_cast<MainWindow*>(this->parent());
    mw->setStride(10);

    // reset shortcuts
    keySeqEditIncrease->setKeySequence(*(std::make_unique<QKeySequence>(Qt::Key_F6).get()));
    keySeqEditDecrease->setKeySequence(*(std::make_unique<QKeySequence>(Qt::Key_F5).get()));
    QKeySequence keySeqIn = keySeqEditIncrease->keySequence();
    QKeySequence keySeqDe = keySeqEditDecrease->keySequence();
    mw->setShortcuts(&keySeqIn, &keySeqDe);
}

void PreferencesWindow::showCentered() {
    this->move(posX, posY);
    this->show();
}
