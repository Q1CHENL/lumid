//
// Created by liuqichen on 2/19/23.
//

#ifndef DARKERBRIGHTNESS_SLIDERSHBOXLAYOUT_H
#define DARKERBRIGHTNESS_SLIDERSHBOXLAYOUT_H


#include <QHBoxLayout>

class SlidersHBoxLayout : public QHBoxLayout{
public:
    QSize sizeHint() const override;
};


#endif //DARKERBRIGHTNESS_SLIDERSHBOXLAYOUT_H
