/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "pointpropertymanager_p.h"
#include "propertymanager/pointpropertymanager.h"
#include <QtCore/QPoint>

PointPropertyManagerPrivate::PointPropertyManagerPrivate()
{

}

void PointPropertyManagerPrivate::slotIntChanged(Property* property, int value)
{
    if (auto xprop = xToProperty_.value(property, nullptr))
    {
        auto p = values_[xprop];
        p.setX(value);
        pointPropertyManagerPtr_->setValue(xprop, p);
    }
    else if (auto yprop = yToProperty_.value(property, nullptr))
    {
        auto p = values_[yprop];
        p.setY(value);
        pointPropertyManagerPtr_->setValue(yprop, p);
    }
}

void PointPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
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
