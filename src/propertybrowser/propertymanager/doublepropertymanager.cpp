/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "doublepropertymanager.h"
#include "private/doublepropertymanager_p.h"
#include "private/propertymanagerutils_p.h"

DoublePropertyManager::DoublePropertyManager(QObject *parent) :
    AbstractPropertyManager(parent),
    impl_(new DoublePropertyManagerPrivate)
{
    impl_->doublePropertyManagerPtr_ = this;
}

DoublePropertyManager::~DoublePropertyManager()
{
    clear();
}

double DoublePropertyManager::value(const Property* property) const
{
    return getValue<double>(impl_->values_, property, 0.0);
}

double DoublePropertyManager::minimum(const Property* property) const
{
    return getMinimum<double>(impl_->values_, property, 0.0);
}

double DoublePropertyManager::maximum(const Property* property) const
{
    return getMaximum<double>(impl_->values_, property, 0.0);
}

double DoublePropertyManager::singleStep(const Property* property) const
{
    return getData<double>(impl_->values_, &DoublePropertyManagerPrivate::Data::singleStep, property, 0);
}

int DoublePropertyManager::decimals(const Property* property) const
{
    return getData<int>(impl_->values_, &DoublePropertyManagerPrivate::Data::decimals, property, 0);
}

QString DoublePropertyManager::valueText(const Property *property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return QString::number(it.value().val, 'f', it.value().decimals);
}

void DoublePropertyManager::setValue(Property* property, double val)
{
    void (DoublePropertyManagerPrivate::*setSubPropertyValue)(Property*, double) = 0;
    setValueInRange<double, DoublePropertyManagerPrivate, DoublePropertyManager, double>(this, impl_.data(),
        &DoublePropertyManager::propertyChanged,
        &DoublePropertyManager::valueChanged,
        property, val, setSubPropertyValue);
}

void DoublePropertyManager::setSingleStep(Property* property, double step)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    if (step < 0)
    {
        step = 0;
    }
    if (data.singleStep == step)
    {
        return;
    }
    data.singleStep = step;

    it.value() = data;

    emit singleStepChanged(property, data.singleStep);
}

void DoublePropertyManager::setDecimals(Property* property, int prec)
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

    it.value() = data;

    emit decimalsChanged(property, data.decimals);
}

void DoublePropertyManager::setMinimum(Property* property, double minVal)
{
    setMinimumValue<double, DoublePropertyManagerPrivate, DoublePropertyManager, double, DoublePropertyManagerPrivate::Data>(this, impl_.data(),
        &DoublePropertyManager::propertyChanged,
        &DoublePropertyManager::valueChanged,
        &DoublePropertyManager::rangeChanged,
        property, minVal);
}

void DoublePropertyManager::setMaximum(Property* property, double maxVal)
{
    setMaximumValue<double, DoublePropertyManagerPrivate, DoublePropertyManager, double, DoublePropertyManagerPrivate::Data>(this, impl_.data(),
        &DoublePropertyManager::propertyChanged,
        &DoublePropertyManager::valueChanged,
        &DoublePropertyManager::rangeChanged,
        property, maxVal);
}

void DoublePropertyManager::setRange(Property* property, double minVal, double maxVal)
{
    void (DoublePropertyManagerPrivate::*setSubPropertyRange)(Property *, double, double, double) = 0;
    setBorderValues<double, DoublePropertyManagerPrivate, DoublePropertyManager, double>(this, impl_.data(),
        &DoublePropertyManager::propertyChanged,
        &DoublePropertyManager::valueChanged,
        &DoublePropertyManager::rangeChanged,
        property, minVal, maxVal, setSubPropertyRange);
}

void DoublePropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};
}

void DoublePropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
