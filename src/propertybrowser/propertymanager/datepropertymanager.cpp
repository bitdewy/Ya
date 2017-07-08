/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "datepropertymanager.h"
#include "private/datepropertymanager_p.h"
#include "private/propertymanagerutils_p.h"

DatePropertyManager::DatePropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new DatePropertyManagerPrivate(this))
{
}

DatePropertyManager::~DatePropertyManager()
{
    clear();
}

QDate DatePropertyManager::value(const Property* property) const
{
    return getValue<QDate>(impl_->values_, property);
}

QDate DatePropertyManager::minimum(const Property* property) const
{
    return getMinimum<QDate>(impl_->values_, property);
}

QDate DatePropertyManager::maximum(const Property* property) const
{
    return getMaximum<QDate>(impl_->values_, property);
}

QString DatePropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return it.value().val.toString(impl_->format_);
}

void DatePropertyManager::setValue(Property* property, const QDate& val)
{
    void (DatePropertyManagerPrivate::*setSubPropertyValue)(Property*, const QDate&) = 0;
    setValueInRange<const QDate&, DatePropertyManagerPrivate, DatePropertyManager, const QDate>(this, impl_.data(),
        &DatePropertyManager::propertyChanged,
        &DatePropertyManager::valueChanged,
        property, val, setSubPropertyValue);
}

void DatePropertyManager::setMinimum(Property* property, const QDate& minVal)
{
    setMinimumValue<const QDate&, DatePropertyManagerPrivate, DatePropertyManager, QDate, DatePropertyManagerPrivate::Data>(this, impl_.data(),
        &DatePropertyManager::propertyChanged,
        &DatePropertyManager::valueChanged,
        &DatePropertyManager::rangeChanged,
        property, minVal);
}

void DatePropertyManager::setMaximum(Property* property, const QDate& maxVal)
{
    setMaximumValue<const QDate&, DatePropertyManagerPrivate, DatePropertyManager, QDate, DatePropertyManagerPrivate::Data>(this, impl_.data(),
        &DatePropertyManager::propertyChanged,
        &DatePropertyManager::valueChanged,
        &DatePropertyManager::rangeChanged,
        property, maxVal);
}

void DatePropertyManager::setRange(Property* property, const QDate& minVal, const QDate& maxVal)
{
    void (DatePropertyManagerPrivate::*setSubPropertyRange)(Property*, const QDate&, const QDate&, const QDate&) = 0;
    setBorderValues<const QDate&, DatePropertyManagerPrivate, DatePropertyManager, QDate>(this, impl_.data(),
        &DatePropertyManager::propertyChanged,
        &DatePropertyManager::valueChanged,
        &DatePropertyManager::rangeChanged,
        property, minVal, maxVal, setSubPropertyRange);
}

void DatePropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = DatePropertyManagerPrivate::Data();
}

void DatePropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
