/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "rectpropertymanager_p.h"
#include "propertymanager/rectpropertymanager.h"
#include "propertymanager/intpropertymanager.h"

void RectPropertyManagerPrivate::slotIntChanged(Property* property, int value)
{
    if (auto prop = xToProperty_.value(property, nullptr))
    {
        auto r = values_[prop].val;
        r.moveLeft(value);
        q_ptr->setValue(prop, r);
    }
    else if (auto prop = yToProperty_.value(property))
    {
        auto r = values_[prop].val;
        r.moveTop(value);
        q_ptr->setValue(prop, r);
    }
    else if (auto prop = wToProperty_.value(property, nullptr))
    {
        auto data = values_[prop];
        auto r = data.val;
        r.setWidth(value);
        if (!data.constraint.isNull() && data.constraint.x() + data.constraint.width() < r.x() + r.width())
        {
            r.moveLeft(data.constraint.left() + data.constraint.width() - r.width());
        }
        q_ptr->setValue(prop, r);
    }
    else if (auto prop = hToProperty_.value(property, nullptr))
    {
        auto data = values_[prop];
        auto r = data.val;
        r.setHeight(value);
        if (!data.constraint.isNull() && data.constraint.y() + data.constraint.height() < r.y() + r.height())
        {
            r.moveTop(data.constraint.top() + data.constraint.height() - r.height());
        }
        q_ptr->setValue(prop, r);
    }
}

void RectPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
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
    else if (auto pointProp = wToProperty_.value(property, nullptr))
    {
        propertyToW_[pointProp] = nullptr;
        wToProperty_.remove(property);
    }
    else if (auto pointProp = hToProperty_.value(property, nullptr))
    {
        propertyToH_[pointProp] = nullptr;
        hToProperty_.remove(property);
    }
}

void RectPropertyManagerPrivate::setConstraint(Property* property,
    const QRect& constraint, const QRect& val)
{
    const auto isNull = constraint.isNull();
    const auto left   = isNull ? INT_MIN : constraint.left();
    const auto right  = isNull ? INT_MAX : constraint.left() + constraint.width();
    const auto top    = isNull ? INT_MIN : constraint.top();
    const auto bottom = isNull ? INT_MAX : constraint.top() + constraint.height();
    const auto width  = isNull ? INT_MAX : constraint.width();
    const auto height = isNull ? INT_MAX : constraint.height();

    intPropertyManager_->setRange(propertyToX_[property], left, right);
    intPropertyManager_->setRange(propertyToY_[property], top, bottom);
    intPropertyManager_->setRange(propertyToW_[property], 0, width);
    intPropertyManager_->setRange(propertyToH_[property], 0, height);

    intPropertyManager_->setValue(propertyToX_[property], val.x());
    intPropertyManager_->setValue(propertyToY_[property], val.y());
    intPropertyManager_->setValue(propertyToW_[property], val.width());
    intPropertyManager_->setValue(propertyToH_[property], val.height());
}
