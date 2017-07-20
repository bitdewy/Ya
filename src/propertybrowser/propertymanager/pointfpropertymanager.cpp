/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "pointfpropertymanager.h"
#include "private/pointfpropertymanager_p.h"
#include "propertymanager/doublepropertymanager.h"
#include "propertymanager/private/propertymanagerutils_p.h"
#include "property.h"

PointFPropertyManager::PointFPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new PointFPropertyManagerPrivate)
{
    impl_->pointFPropertyManagerPtr_ = this;

    impl_->doublePropertyManager_ = new DoublePropertyManager(this);
    connect(impl_->doublePropertyManager_, &DoublePropertyManager::valueChanged,
        this, [this](Property* property, double value) { impl_->slotDoubleChanged(property, value); });
    connect(impl_->doublePropertyManager_, &DoublePropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

PointFPropertyManager::~PointFPropertyManager()
{
    clear();
}

DoublePropertyManager *PointFPropertyManager::subDoublePropertyManager() const
{
    return impl_->doublePropertyManager_;
}

QPointF PointFPropertyManager::value(const Property* property) const
{
    return getValue<QPointF>(impl_->values_, property);
}

int PointFPropertyManager::decimals(const Property* property) const
{
    return getData<int>(impl_->values_, &PointFPropertyManagerPrivate::Data::decimals, property, 0);
}

QString PointFPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto v = it.value().val;
    const auto dec =  it.value().decimals;
    return tr("(%1, %2)").arg(QString::number(v.x(), 'f', dec))
                         .arg(QString::number(v.y(), 'f', dec));
}

void PointFPropertyManager::setValue(Property* property, const QPointF& val)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    if (it.value().val == val)
    {
        return;
    }
    it.value().val = val;
    impl_->doublePropertyManager_->setValue(impl_->propertyToX_[property], val.x());
    impl_->doublePropertyManager_->setValue(impl_->propertyToY_[property], val.y());

    emit propertyChanged(property);
    emit valueChanged(property, val);
}

void PointFPropertyManager::setDecimals(Property* property, int prec)
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

    it.value() = data;

    emit decimalsChanged(property, data.decimals);
}

void PointFPropertyManager::initializeProperty(Property *property)
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
}

void PointFPropertyManager::uninitializeProperty(Property* property)
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

    impl_->values_.remove(property);
}
