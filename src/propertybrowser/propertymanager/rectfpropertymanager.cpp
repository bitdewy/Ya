/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "rectfpropertymanager.h"
#include "private/rectfpropertymanager_p.h"
#include "propertymanager/doublepropertymanager.h"
#include "propertymanager/private/propertymanagerutils_p.h"
#include "property.h"

RectFPropertyManager::RectFPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new RectFPropertyManagerPrivate)
{
    impl_->rectFPropertyManagerPtr_ = this;

    impl_->doublePropertyManager_ = new DoublePropertyManager(this);
    connect(impl_->doublePropertyManager_, &DoublePropertyManager::valueChanged,
                this, [this](Property* property, double value) { impl_->slotDoubleChanged(property, value); });
    connect(impl_->doublePropertyManager_, &DoublePropertyManager::propertyDestroyed,
                this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

RectFPropertyManager::~RectFPropertyManager()
{
    clear();
}

DoublePropertyManager* RectFPropertyManager::subDoublePropertyManager() const
{
    return impl_->doublePropertyManager_;
}

QRectF RectFPropertyManager::value(const Property* property) const
{
    return getValue<QRectF>(impl_->values_, property);
}

int RectFPropertyManager::decimals(const Property* property) const
{
    return getData<int>(impl_->values_, &RectFPropertyManagerPrivate::Data::decimals, property, 0);
}

QRectF RectFPropertyManager::constraint(const Property* property) const
{
    return getData<QRectF>(impl_->values_, &RectFPropertyManagerPrivate::Data::constraint, property, QRect());
}

QString RectFPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto v = it.value().val;
    const auto dec = it.value().decimals;
    return QString(tr("[(%1, %2), %3 x %4]").arg(QString::number(v.x(), 'f', dec))
                                .arg(QString::number(v.y(), 'f', dec))
                                .arg(QString::number(v.width(), 'f', dec))
                                .arg(QString::number(v.height(), 'f', dec)));
}

void RectFPropertyManager::setValue(Property* property, const QRectF& val)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    auto newRect = val.normalized();
    if (!data.constraint.isNull() && !data.constraint.contains(newRect))
    {
        const auto r1 = data.constraint;
        const auto r2 = newRect;
        newRect.setLeft(qMax(r1.left(), r2.left()));
        newRect.setRight(qMin(r1.right(), r2.right()));
        newRect.setTop(qMax(r1.top(), r2.top()));
        newRect.setBottom(qMin(r1.bottom(), r2.bottom()));
        if (newRect.width() < 0 || newRect.height() < 0)
        {
            return;
        }
    }

    if (data.val == newRect)
    {
        return;
    }
    data.val = newRect;

    it.value() = data;
    impl_->doublePropertyManager_->setValue(impl_->propertyToX_[property], newRect.x());
    impl_->doublePropertyManager_->setValue(impl_->propertyToY_[property], newRect.y());
    impl_->doublePropertyManager_->setValue(impl_->propertyToW_[property], newRect.width());
    impl_->doublePropertyManager_->setValue(impl_->propertyToH_[property], newRect.height());

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void RectFPropertyManager::setConstraint(Property* property, const QRectF& constraint)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    auto newConstraint = constraint.normalized();
    if (data.constraint == newConstraint)
    {
        return;
    }
    const auto oldVal = data.val;

    data.constraint = newConstraint;

    if (!data.constraint.isNull() && !data.constraint.contains(oldVal))
    {
        auto r1 = data.constraint;
        auto r2 = data.val;

        if (r2.width() > r1.width())
        {
            r2.setWidth(r1.width());
        }
        if (r2.height() > r1.height())
        {
            r2.setHeight(r1.height());
        }
        if (r2.left() < r1.left())
        {
            r2.moveLeft(r1.left());
        }
        else if (r2.right() > r1.right())
        {
            r2.moveRight(r1.right());
        }
        if (r2.top() < r1.top())
        {
            r2.moveTop(r1.top());
        }
        else if (r2.bottom() > r1.bottom())
        {
            r2.moveBottom(r1.bottom());
        }
        data.val = r2;
    }

    it.value() = data;

    emit constraintChanged(property, data.constraint);

    impl_->setConstraint(property, data.constraint, data.val);

    if (data.val == oldVal)
    {
        return;
    }
    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void RectFPropertyManager::setDecimals(Property* property, int prec)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    if (prec > 13)
    {
        prec = 13;
    }
    else if (prec < 0)
    {
        prec = 0;
    }
    if (data.decimals == prec)
    {
        return;
    }
    data.decimals = prec;
    impl_->doublePropertyManager_->setDecimals(impl_->propertyToX_[property], prec);
    impl_->doublePropertyManager_->setDecimals(impl_->propertyToY_[property], prec);
    impl_->doublePropertyManager_->setDecimals(impl_->propertyToW_[property], prec);
    impl_->doublePropertyManager_->setDecimals(impl_->propertyToH_[property], prec);

    it.value() = data;

    emit decimalsChanged(property, data.decimals);
}

void RectFPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};

    auto xProp = impl_->doublePropertyManager_->addProperty();
    xProp->setPropertyName(tr("X"));
    impl_->doublePropertyManager_->setDecimals(xProp, decimals(property));
    impl_->doublePropertyManager_->setValue(xProp, 0);
    impl_->propertyToX_[property] = xProp;
    impl_->xToProperty_[xProp] = property;
    property->addSubProperty(xProp);

    auto yProp = impl_->doublePropertyManager_->addProperty();
    yProp->setPropertyName(tr("Y"));
    impl_->doublePropertyManager_->setDecimals(yProp, decimals(property));
    impl_->doublePropertyManager_->setValue(yProp, 0);
    impl_->propertyToY_[property] = yProp;
    impl_->yToProperty_[yProp] = property;
    property->addSubProperty(yProp);

    auto wProp = impl_->doublePropertyManager_->addProperty();
    wProp->setPropertyName(tr("Width"));
    impl_->doublePropertyManager_->setDecimals(wProp, decimals(property));
    impl_->doublePropertyManager_->setValue(wProp, 0);
    impl_->doublePropertyManager_->setMinimum(wProp, 0);
    impl_->propertyToW_[property] = wProp;
    impl_->wToProperty_[wProp] = property;
    property->addSubProperty(wProp);

    auto hProp = impl_->doublePropertyManager_->addProperty();
    hProp->setPropertyName(tr("Height"));
    impl_->doublePropertyManager_->setDecimals(hProp, decimals(property));
    impl_->doublePropertyManager_->setValue(hProp, 0);
    impl_->doublePropertyManager_->setMinimum(hProp, 0);
    impl_->propertyToH_[property] = hProp;
    impl_->hToProperty_[hProp] = property;
    property->addSubProperty(hProp);
}

void RectFPropertyManager::uninitializeProperty(Property* property)
{
    auto xProp = impl_->propertyToX_[property];
    if (xProp)
    {
        impl_->xToProperty_.remove(xProp);
        delete xProp;
    }
    impl_->propertyToX_.remove(property);

    auto yProp = impl_->propertyToY_[property];
    if (yProp)
    {
        impl_->yToProperty_.remove(yProp);
        delete yProp;
    }
    impl_->propertyToY_.remove(property);

    auto wProp = impl_->propertyToW_[property];
    if (wProp)
    {
        impl_->wToProperty_.remove(wProp);
        delete wProp;
    }
    impl_->propertyToW_.remove(property);

    auto hProp = impl_->propertyToH_[property];
    if (hProp)
    {
        impl_->hToProperty_.remove(hProp);
        delete hProp;
    }
    impl_->propertyToH_.remove(property);

    impl_->values_.remove(property);
}
