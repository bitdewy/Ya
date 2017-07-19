/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "sizepropertymanager_p.h"
#include "propertymanager/sizepropertymanager.h"
#include "propertymanager/intpropertymanager.h"
#include "propertymanager/private/propertymanagerutils_p.h"

void SizePropertyManagerPrivate::slotIntChanged(Property* property, int value)
{
    if (auto prop = wToProperty_.value(property, 0))
    {
        auto s = values_[prop].val;
        s.setWidth(value);
        q_ptr->setValue(prop, s);
    }
    else if (auto prop = hToProperty_.value(property, 0))
    {
        auto s = values_[prop].val;
        s.setHeight(value);
        q_ptr->setValue(prop, s);
    }
}

void SizePropertyManagerPrivate::slotPropertyDestroyed(Property* property)
{
    if (auto pointProp = wToProperty_.value(property, 0))
    {
        propertyToW_[pointProp] = 0;
        wToProperty_.remove(property);
    }
    else if (auto pointProp = hToProperty_.value(property, 0))
    {
        propertyToH_[pointProp] = 0;
        hToProperty_.remove(property);
    }
}

void SizePropertyManagerPrivate::setValue(Property* property, const QSize& val)
{
    intPropertyManager_->setValue(propertyToW_.value(property), val.width());
    intPropertyManager_->setValue(propertyToH_.value(property), val.height());
}

void SizePropertyManagerPrivate::setRange(Property* property,
    const QSize& minVal, const QSize& maxVal, const QSize& val)
{
    auto wProperty = propertyToW_.value(property);
    auto hProperty = propertyToH_.value(property);
    intPropertyManager_->setRange(wProperty, minVal.width(), maxVal.width());
    intPropertyManager_->setValue(wProperty, val.width());
    intPropertyManager_->setRange(hProperty, minVal.height(), maxVal.height());
    intPropertyManager_->setValue(hProperty, val.height());
}

SizePropertyManagerPrivate::Data::Data() :
    val{0, 0},
    minVal{0, 0},
    maxVal{INT_MAX, INT_MAX}
{

}

QSize SizePropertyManagerPrivate::Data::minimumValue() const
{
    return minVal;
}

QSize SizePropertyManagerPrivate::Data::maximumValue() const
{
    return maxVal;
}

void SizePropertyManagerPrivate::Data::setMinimumValue(const QSize& newMinVal)
{
    setSizeMinimumData(this, newMinVal);
}

void SizePropertyManagerPrivate::Data::setMaximumValue(const QSize& newMaxVal)
{
    setSizeMaximumData(this, newMaxVal);
}
