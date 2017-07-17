/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "sizepolicypropertymanager_p.h"
#include "propertymanager/sizepolicypropertymanager.h"
#include "propertymanager/metaenumprovider.h"
#include <QtWidgets/QSizePolicy>

SizePolicyPropertyManagerPrivate::SizePolicyPropertyManagerPrivate()
{
}

void SizePolicyPropertyManagerPrivate::slotIntChanged(Property* property, int value)
{
    if (auto prop = hStretchToProperty_.value(property, nullptr))
    {
        auto sp = values_[prop];
        sp.setHorizontalStretch(value);
        sizePolicyPropertyManager_->setValue(prop, sp);
    }
    else if (auto prop = vStretchToProperty_.value(property, nullptr))
    {
        auto sp = values_[prop];
        sp.setVerticalStretch(value);
        sizePolicyPropertyManager_->setValue(prop, sp);
    }
}

void SizePolicyPropertyManagerPrivate::slotEnumChanged(Property* property, int value)
{
    if (auto prop = hPolicyToProperty_.value(property, nullptr))
    {
        auto sp = values_[prop];
        sp.setHorizontalPolicy(metaEnumProvider()->indexToSizePolicy(value));
        sizePolicyPropertyManager_->setValue(prop, sp);
    }
    else if (auto prop = vPolicyToProperty_.value(property, nullptr))
    {
        auto sp = values_[prop];
        sp.setVerticalPolicy(metaEnumProvider()->indexToSizePolicy(value));
        sizePolicyPropertyManager_->setValue(prop, sp);
    }
}

void SizePolicyPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
{
    if (auto pointProp = hStretchToProperty_.value(property, nullptr))
    {
        propertyToHStretch_[pointProp] = nullptr;
        hStretchToProperty_.remove(property);
    }
    else if (auto pointProp = vStretchToProperty_.value(property, nullptr))
    {
        propertyToVStretch_[pointProp] = nullptr;
        vStretchToProperty_.remove(property);
    }
    else if (auto pointProp = hPolicyToProperty_.value(property, nullptr))
    {
        propertyToHPolicy_[pointProp] = nullptr;
        hPolicyToProperty_.remove(property);
    }
    else if (auto pointProp = vPolicyToProperty_.value(property, nullptr))
    {
        propertyToVPolicy_[pointProp] = nullptr;
        vPolicyToProperty_.remove(property);
    }
}
