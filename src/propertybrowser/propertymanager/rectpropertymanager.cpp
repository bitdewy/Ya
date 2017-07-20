/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "rectpropertymanager.h"
#include "private/rectpropertymanager_p.h"
#include "propertymanager/intpropertymanager.h"
#include "propertymanager/private/propertymanagerutils_p.h"
#include "property.h"

RectPropertyManager::RectPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new RectPropertyManagerPrivate)
{
    impl_->rectPropertyManagerPtr_ = this;

    impl_->intPropertyManager_ = new IntPropertyManager(this);
    connect(impl_->intPropertyManager_, &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value)  { impl_->slotIntChanged(property, value); });
    connect(impl_->intPropertyManager_, &IntPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

RectPropertyManager::~RectPropertyManager()
{
    clear();
}

IntPropertyManager* RectPropertyManager::subIntPropertyManager() const
{
    return impl_->intPropertyManager_;
}

QRect RectPropertyManager::value(const Property* property) const
{
    return getValue<QRect>(impl_->values_, property);
}

QRect RectPropertyManager::constraint(const Property* property) const
{
    return getData<QRect>(impl_->values_, &RectPropertyManagerPrivate::Data::constraint, property, QRect());
}

QString RectPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto v = it.value().val;
    return tr("[(%1, %2), %3 x %4]").arg(QString::number(v.x()))
                                    .arg(QString::number(v.y()))
                                    .arg(QString::number(v.width()))
                                    .arg(QString::number(v.height()));
}

void RectPropertyManager::setValue(Property* property, const QRect& val)
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
    impl_->intPropertyManager_->setValue(impl_->propertyToX_[property], newRect.x());
    impl_->intPropertyManager_->setValue(impl_->propertyToY_[property], newRect.y());
    impl_->intPropertyManager_->setValue(impl_->propertyToW_[property], newRect.width());
    impl_->intPropertyManager_->setValue(impl_->propertyToH_[property], newRect.height());

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void RectPropertyManager::setConstraint(Property* property, const QRect& constraint)
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

void RectPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};

    auto xProp = impl_->intPropertyManager_->addProperty();
    xProp->setPropertyName(tr("X"));
    impl_->intPropertyManager_->setValue(xProp, 0);
    impl_->propertyToX_[property] = xProp;
    impl_->xToProperty_[xProp] = property;
    property->addSubProperty(xProp);

    auto yProp = impl_->intPropertyManager_->addProperty();
    yProp->setPropertyName(tr("Y"));
    impl_->intPropertyManager_->setValue(yProp, 0);
    impl_->propertyToY_[property] = yProp;
    impl_->yToProperty_[yProp] = property;
    property->addSubProperty(yProp);

    auto wProp = impl_->intPropertyManager_->addProperty();
    wProp->setPropertyName(tr("Width"));
    impl_->intPropertyManager_->setValue(wProp, 0);
    impl_->intPropertyManager_->setMinimum(wProp, 0);
    impl_->propertyToW_[property] = wProp;
    impl_->wToProperty_[wProp] = property;
    property->addSubProperty(wProp);

    auto hProp = impl_->intPropertyManager_->addProperty();
    hProp->setPropertyName(tr("Height"));
    impl_->intPropertyManager_->setValue(hProp, 0);
    impl_->intPropertyManager_->setMinimum(hProp, 0);
    impl_->propertyToH_[property] = hProp;
    impl_->hToProperty_[hProp] = property;
    property->addSubProperty(hProp);
}

void RectPropertyManager::uninitializeProperty(Property* property)
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
