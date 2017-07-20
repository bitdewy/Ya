/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "pointfpropertymanager_p.h"
#include "propertymanager/pointfpropertymanager.h"

void PointFPropertyManagerPrivate::slotDoubleChanged(Property* property, double value)
{
    if (auto prop = xToProperty_.value(property, nullptr))
    {
        auto p = values_[prop].val;
        p.setX(value);
        pointFPropertyManagerPtr_->setValue(prop, p);
    }
    else if (auto prop = yToProperty_.value(property, nullptr))
    {
        auto p = values_[prop].val;
        p.setY(value);
        pointFPropertyManagerPtr_->setValue(prop, p);
    }
}

void PointFPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
{
    if (auto pointProp = xToProperty_.value(property, nullptr))
    {
        propertyToX_[pointProp] = nullptr;
        xToProperty_.remove(property);
    }
    else if (auto pointProp = yToProperty_.value(property, nullptr))
    {
        propertyToY_[pointProp] = nullptr;
        yToProperty_.remove(property);
    }
}
