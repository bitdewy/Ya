/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "sizefpropertymanager_p.h"
#include "propertymanager/sizefpropertymanager.h"
#include "propertymanager/doublepropertymanager.h"
#include "propertymanager/private/propertymanagerutils_p.h"

void SizeFPropertyManagerPrivate::slotDoubleChanged(Property* property, double value)
{
    if (auto prop = wToProperty_.value(property, 0))
    {
        auto s = values_[prop].val;
        s.setWidth(value);
        sizeFPropertyManagerPtr_->setValue(prop, s);
    }
    else if (auto prop = hToProperty_.value(property, 0))
    {
        auto s = values_[prop].val;
        s.setHeight(value);
        sizeFPropertyManagerPtr_->setValue(prop, s);
    }
}

void SizeFPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
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

void SizeFPropertyManagerPrivate::setValue(Property* property, const QSizeF& val)
{
    doublePropertyManager_->setValue(propertyToW_.value(property), val.width());
    doublePropertyManager_->setValue(propertyToH_.value(property), val.height());
}

void SizeFPropertyManagerPrivate::setRange(Property* property,
    const QSizeF& minVal, const QSizeF& maxVal, const QSizeF& val)
{
    doublePropertyManager_->setRange(propertyToW_[property], minVal.width(), maxVal.width());
    doublePropertyManager_->setValue(propertyToW_[property], val.width());
    doublePropertyManager_->setRange(propertyToH_[property], minVal.height(), maxVal.height());
    doublePropertyManager_->setValue(propertyToH_[property], val.height());
}

QSizeF SizeFPropertyManagerPrivate::Data::minimumValue() const
{
    return minVal;
}

QSizeF SizeFPropertyManagerPrivate::Data::maximumValue() const
{
    return maxVal;
}

void SizeFPropertyManagerPrivate::Data::setMinimumValue(const QSizeF& newMinVal)
{
    setSizeMinimumData(this, newMinVal);
}

void SizeFPropertyManagerPrivate::Data::setMaximumValue(const QSizeF& newMaxVal)
{
    setSizeMaximumData(this, newMaxVal);
}
