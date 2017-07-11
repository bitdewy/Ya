/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2nullptr17, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "colorpropertymanager_p.h"
#include "propertymanager/colorpropertymanager.h"
#include <QtGui/QColor>

ColorPropertyManagerPrivate::ColorPropertyManagerPrivate()
{

}

ColorPropertyManagerPrivate::~ColorPropertyManagerPrivate()
{

}

void ColorPropertyManagerPrivate::slotIntChanged(Property* property, int value)
{
    if (auto prop = rToProperty_.value(property, nullptr))
    {
        auto c = values_[prop];
        c.setRed(value);
        colorPropertyManagerPtr_->setValue(prop, c);
    }
    else if (auto prop = gToProperty_.value(property, nullptr))
    {
        auto c = values_[prop];
        c.setGreen(value);
        colorPropertyManagerPtr_->setValue(prop, c);
    }
    else if (auto prop = bToProperty_.value(property, nullptr))
    {
        auto c = values_[prop];
        c.setBlue(value);
        colorPropertyManagerPtr_->setValue(prop, c);
    }
    else if (auto prop = aToProperty_.value(property, nullptr))
    {
        auto c = values_[prop];
        c.setAlpha(value);
        colorPropertyManagerPtr_->setValue(prop, c);
    }
}

void ColorPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
{
    if (auto pointProp = rToProperty_.value(property, nullptr))
    {
        propertyToR_[pointProp] = nullptr;
        rToProperty_.remove(property);
    }
    else if (auto pointProp = gToProperty_.value(property, nullptr))
    {
        propertyToG_[pointProp] = nullptr;
        gToProperty_.remove(property);
    }
    else if (auto pointProp = bToProperty_.value(property, nullptr))
    {
        propertyToB_[pointProp] = nullptr;
        bToProperty_.remove(property);
    }
    else if (auto pointProp = aToProperty_.value(property, nullptr))
    {
        propertyToA_[pointProp] = nullptr;
        aToProperty_.remove(property);
    }
}
