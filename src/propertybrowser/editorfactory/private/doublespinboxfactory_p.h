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
#include <QtWidgets/QDoubleSpinBox>

class DoubleSpinBoxFactory;
class DoubleSpinBoxFactoryPrivate : public EditorFactoryPrivate<QDoubleSpinBox>
{
    DoubleSpinBoxFactory* doubleSpinBoxFactoryPtr_;
    friend class DoubleSpinBoxFactory;
public:

    void slotPropertyChanged(Property* property, double value);
    void slotRangeChanged(Property* property, double min, double max);
    void slotSingleStepChanged(Property* property, double step);
    void slotDecimalsChanged(Property* property, int prec);
    void slotSetValue(double value);
};
