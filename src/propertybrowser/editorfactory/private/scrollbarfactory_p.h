/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "editorfactory_p.h"
#include <QtWidgets/QScrollBar>

class ScrollBarFactory;
class ScrollBarFactoryPrivate : public  EditorFactoryPrivate<QScrollBar>
{
    ScrollBarFactory* scrollBarFactoryPtr_;
    friend class ScrollBarFactory;
public:
    void slotPropertyChanged(Property* property, int value);
    void slotRangeChanged(Property* property, int min, int max);
    void slotSingleStepChanged(Property* property, int step);
    void slotSetValue(int value);
};
