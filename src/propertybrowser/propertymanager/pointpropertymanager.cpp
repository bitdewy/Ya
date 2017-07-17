/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "pointpropertymanager.h"
#include "private/pointpropertymanager_p.h"
#include "propertymanager/intpropertymanager.h"
#include "property.h"
#include <QtCore/QPoint>

PointPropertyManager::PointPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new PointPropertyManagerPrivate)
{
    impl_->pointPropertyManagerPtr_ = this;

    impl_->intPropertyManager_ = new IntPropertyManager(this);
    connect(impl_->intPropertyManager_, &IntPropertyManager::valueChanged,
        this, [this](Property* property, int value) { impl_->slotIntChanged(property, value); });
    connect(impl_->intPropertyManager_, &IntPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

PointPropertyManager::~PointPropertyManager()
{
    clear();
}

IntPropertyManager *PointPropertyManager::subIntPropertyManager() const
{
    return impl_->intPropertyManager_;
}

QPoint PointPropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QPoint());
}

QString PointPropertyManager::valueText(const Property *property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto v = it.value();
    return tr("(%1, %2)").arg(QString::number(v.x()))
                         .arg(QString::number(v.y()));
}

void PointPropertyManager::setValue(Property* property, const QPoint& val)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    if (it.value() == val)
    {
        return;
    }
    it.value() = val;
    impl_->intPropertyManager_->setValue(impl_->propertyToX_[property], val.x());
    impl_->intPropertyManager_->setValue(impl_->propertyToY_[property], val.y());

    emit propertyChanged(property);
    emit valueChanged(property, val);
}

void PointPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = QPoint(0, 0);

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
}

void PointPropertyManager::uninitializeProperty(Property* property)
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
