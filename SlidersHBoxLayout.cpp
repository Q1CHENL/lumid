//
// Created by liuqichen on 2/19/23.
//

#include "SlidersHBoxLayout.h"
QSize SlidersHBoxLayout::sizeHint() const
{
QSize hint = QHBoxLayout::sizeHint();
hint.setHeight(300); // Set a custom width for the layout
return hint;
}