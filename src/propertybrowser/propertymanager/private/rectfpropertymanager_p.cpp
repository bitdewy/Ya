/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "rectfpropertymanager_p.h"
#include "propertymanager/rectfpropertymanager.h"
#include "propertymanager/doublepropertymanager.h"
#include <cfloat>

void RectFPropertyManagerPrivate::slotDoubleChanged(Property* property, double value)
{
    if (auto prop = xToProperty_.value(property, 0))
    {
        auto r = values_[prop].val;
        r.moveLeft(value);
        rectFPropertyManagerPtr_->setValue(prop, r);
    }
    else if (auto prop = yToProperty_.value(property, 0))
    {
        auto r = values_[prop].val;
        r.moveTop(value);
        rectFPropertyManagerPtr_->setValue(prop, r);
    }
    else if (auto prop = wToProperty_.value(property, 0))
    {
        auto data = values_[prop];
        auto r = data.val;
        r.setWidth(value);
        if (!data.constraint.isNull() && data.constraint.x() + data.constraint.width() < r.x() + r.width())
        {
            r.moveLeft(data.constraint.left() + data.constraint.width() - r.width());
        }
        rectFPropertyManagerPtr_->setValue(prop, r);
    }
    else if (auto prop = hToProperty_.value(property, 0))
    {
        auto data = values_[prop];
        auto r = data.val;
        r.setHeight(value);
        if (!data.constraint.isNull() && data.constraint.y() + data.constraint.height() < r.y() + r.height())
        {
            r.moveTop(data.constraint.top() + data.constraint.height() - r.height());
        }
        rectFPropertyManagerPtr_->setValue(prop, r);
    }
}

void RectFPropertyManagerPrivate::slotPropertyDestroyed(Property* property)
{
    if (auto pointProp = xToProperty_.value(property, 0))
    {
        propertyToX_[pointProp] = 0;
        xToProperty_.remove(property);
    }
    else if (auto pointProp = yToProperty_.value(property, 0))
    {
        propertyToY_[pointProp] = 0;
        yToProperty_.remove(property);
    }
    else if (auto pointProp = wToProperty_.value(property, 0))
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

void RectFPropertyManagerPrivate::setConstraint(Property* property,
    const QRectF& constraint, const QRectF& val)
{
    const auto isNull = constraint.isNull();
    const auto left   = isNull ? FLT_MIN : constraint.left();
    const auto right  = isNull ? FLT_MAX : constraint.left() + constraint.width();
    const auto top    = isNull ? FLT_MIN : constraint.top();
    const auto bottom = isNull ? FLT_MAX : constraint.top() + constraint.height();
    const auto width  = isNull ? FLT_MAX : constraint.width();
    const auto height = isNull ? FLT_MAX : constraint.height();

    doublePropertyManager_->setRange(propertyToX_[property], left, right);
    doublePropertyManager_->setRange(propertyToY_[property], top, bottom);
    doublePropertyManager_->setRange(propertyToW_[property], 0, width);
    doublePropertyManager_->setRange(propertyToH_[property], 0, height);

    doublePropertyManager_->setValue(propertyToX_[property], val.x());
    doublePropertyManager_->setValue(propertyToY_[property], val.y());
    doublePropertyManager_->setValue(propertyToW_[property], val.width());
    doublePropertyManager_->setValue(propertyToH_[property], val.height());
}

