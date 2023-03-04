//
// Created by liuqichen on 2/19/23.
//

#ifndef DARKERBRIGHTNESS_SLIDERSHBOXLAYOUT_HPP
#define DARKERBRIGHTNESS_SLIDERSHBOXLAYOUT_HPP


#include <QHBoxLayout>

class SlidersHBoxLayout : public QHBoxLayout{
public:
    //[[nodiscard]] is an attribute specifier in C++ that indicates that a function's return value should not be ignored.
    [[nodiscard]] QSize sizeHint() const override;
};

#endif //DARKERBRIGHTNESS_SLIDERSHBOXLAYOUT_HPP
