// First Inclusion: The first time this header is included in a .cpp file, MYCLASS_H is not
// yet defined, so the code enters the block between #ifndef MYCLASS_H and #endif.

// The preprocessor directive #define MYCLASS_H is encountered, defining MYCLASS_H.
// The actual content of the header file (your class definition, function declarations, etc.)
// is included.
// Subsequent Inclusions: If the same header is included again, either in the same .cpp file
// or in another one that gets linked with the first, the #ifndef MYCLASS_H check will fail,
// because MYCLASS_H has already been defined.

// The content between #ifndef MYCLASS_H and #endif will be skipped, effectively preventing
// multiple inclusions of the same code.

#ifndef LUMID_PREFERENCESWINDOW_HPP
#define LUMID_PREFERENCESWINDOW_HPP

#include <QAction>
#include <QDialog>
#include <QHBoxLayout>
#include <QKeySequenceEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

class PreferencesWindow : public QDialog {
   public:

    // In C++, you generally only provide default values
    // for arguments in the function declaration, not in
    // the function definition.
    PreferencesWindow();
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent* event) override;
    int getStride() const;
    QPushButton* applyButton;
    QPushButton* resetButton;
    QSpinBox* spinbox;
    // keybinding edit fields
    QKeySequenceEdit* keySeqEditIncrease;
    QKeySequenceEdit* keySeqEditDecrease;

   private:
    int posX, posY;

    QLabel* strideLabel;
    QLabel* increaseLabel;
    QLabel* decreaseLabel;

    // Qt will manage their lifetime
    QHBoxLayout* strideLayout;
    QHBoxLayout* increaseLayout;
    QHBoxLayout* decreaseLayout;
    QHBoxLayout* bottomButtonLayout;
    QVBoxLayout* mainLayout;

   private slots:
    void reset();
};

#endif