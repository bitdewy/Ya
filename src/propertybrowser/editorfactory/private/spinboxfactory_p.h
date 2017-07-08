/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "editorfactory/private/editorfactory_p.h"
#include <QtWidgets/QSpinBox>

class SpinBoxFactory;
class SpinBoxFactoryPrivate : public EditorFactoryPrivate<QSpinBox>
{
    SpinBoxFactory* spinBoxFactoryPtr_;
    friend class SpinBoxFactory;

public:
    void slotPropertyChanged(Property* property, int value);
    void slotRangeChanged(Property* property, int min, int max);
    void slotSingleStepChanged(Property* property, int step);
    void slotSetValue(int value);
};
