/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "intpropertymanager.h"
#include "private/intpropertymanager_p.h"
#include "private/propertymanagerutils_p.h"

IntPropertyManager::IntPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new IntPropertyManagerPrivate)
{
    impl_->intPropertyManagerPtr_ = this;
}

IntPropertyManager::~IntPropertyManager()
{
    clear();
}

int IntPropertyManager::value(const Property* property) const
{
    return getValue<int>(impl_->values_, property, 0);
}

int IntPropertyManager::minimum(const Property* property) const
{
    return getMinimum<int>(impl_->values_, property, 0);
}

int IntPropertyManager::maximum(const Property* property) const
{
    return getMaximum<int>(impl_->values_, property, 0);
}

int IntPropertyManager::singleStep(const Property* property) const
{
    return getData<int>(impl_->values_, &IntPropertyManagerPrivate::Data::singleStep, property, 0);
}

QString IntPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return QString::number(it.value().val);
}

void IntPropertyManager::setValue(Property *property, int val)
{
    void (IntPropertyManagerPrivate::*setSubPropertyValue)(Property*, int) = 0;
    setValueInRange<int, IntPropertyManagerPrivate, IntPropertyManager, int>(this, impl_.data(),
                &IntPropertyManager::propertyChanged,
                &IntPropertyManager::valueChanged,
                property, val, setSubPropertyValue);
}

void IntPropertyManager::setMinimum(Property* property, int minVal)
{
    setMinimumValue<int, IntPropertyManagerPrivate, IntPropertyManager, int, IntPropertyManagerPrivate::Data>(this, impl_.data(),
                &IntPropertyManager::propertyChanged,
                &IntPropertyManager::valueChanged,
                &IntPropertyManager::rangeChanged,
                property, minVal);
}

void IntPropertyManager::setMaximum(Property* property, int maxVal)
{
    setMaximumValue<int, IntPropertyManagerPrivate, IntPropertyManager, int, IntPropertyManagerPrivate::Data>(this, impl_.data(),
                &IntPropertyManager::propertyChanged,
                &IntPropertyManager::valueChanged,
                &IntPropertyManager::rangeChanged,
                property, maxVal);
}

void IntPropertyManager::setRange(Property* property, int minVal, int maxVal)
{
    void (IntPropertyManagerPrivate::*setSubPropertyRange)(Property*, int, int, int) = 0;
    setBorderValues<int, IntPropertyManagerPrivate, IntPropertyManager, int>(this, impl_.data(),
                &IntPropertyManager::propertyChanged,
                &IntPropertyManager::valueChanged,
                &IntPropertyManager::rangeChanged,
                property, minVal, maxVal, setSubPropertyRange);
}

void IntPropertyManager::setSingleStep(Property* property, int step)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
        return;

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

void IntPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = IntPropertyManagerPrivate::Data();
}

void IntPropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
