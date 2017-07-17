/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "flagpropertymanager_p.h"
#include "propertymanager/flagpropertymanager.h"

FlagPropertyManagerPrivate::FlagPropertyManagerPrivate()
{

}

void FlagPropertyManagerPrivate::slotBoolChanged(Property* property, bool value)
{
    auto prop = flagToProperty_.value(property, 0);
    if (!prop)
    {
        return;
    }
    const auto pfit = propertyToFlags_.constFind(prop);
    if (pfit == propertyToFlags_.constEnd())
    {
        return;
    }
    auto level = 0;
    for (auto&& p : pfit.value())
    {
        if (p == property)
        {
            auto v = values_[prop].val;
            if (value)
            {
                v |= (1 << level);
            }
            else
            {
                v &= ~(1 << level);
            }
            flagPropertyManagerPtr_->setValue(prop, v);
            return;
        }
        level++;
    }
}

void FlagPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
{
    auto flagProperty = flagToProperty_.value(property, 0);
    if (!flagProperty)
    {
        return;
    }
    propertyToFlags_[flagProperty].replace(propertyToFlags_[flagProperty].indexOf(property), 0);
    flagToProperty_.remove(property);
}
