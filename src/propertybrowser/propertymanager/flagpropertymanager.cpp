/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "flagpropertymanager.h"
#include "private/flagpropertymanager_p.h"
#include "propertymanager/boolpropertymanager.h"
#include "private/propertymanagerutils_p.h"
#include "property.h"

FlagPropertyManager::FlagPropertyManager(QObject *parent) :
    AbstractPropertyManager(parent),
    impl_(new FlagPropertyManagerPrivate)
{
    impl_->flagPropertyManagerPtr_ = this;

    impl_->boolPropertyManager_ = new BoolPropertyManager(this);
    connect(impl_->boolPropertyManager_, &BoolPropertyManager::valueChanged,
        this, [this](Property* property, bool value) { impl_->slotBoolChanged(property, value); });
    connect(impl_->boolPropertyManager_, &BoolPropertyManager::propertyDestroyed,
        this, [this](Property* property) { impl_->slotPropertyDestroyed(property); });
}

FlagPropertyManager::~FlagPropertyManager()
{
    clear();
}

BoolPropertyManager* FlagPropertyManager::subBoolPropertyManager() const
{
    return impl_->boolPropertyManager_;
}

int FlagPropertyManager::value(const Property* property) const
{
    return getValue<int>(impl_->values_, property, 0);
}

QStringList FlagPropertyManager::flagNames(const Property* property) const
{
    return getData<QStringList>(impl_->values_, &FlagPropertyManagerPrivate::Data::flagNames, property, QStringList());
}

QString FlagPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return {};
    }
    const auto& data = it.value();

    QString str;
    auto level = 0;
    const auto bar = QLatin1Char('|');
    const auto fncend = data.flagNames.constEnd();
    for (auto it =  data.flagNames.constBegin(); it != fncend; ++it)
    {
        if (data.val & (1 << level))
        {
            if (!str.isEmpty())
            {
                str += bar;
            }
            str += *it;
        }

        level++;
    }
    return str;
}

void FlagPropertyManager::setValue(Property* property, int val)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    if (data.val == val)
    {
        return;
    }
    if (val > (1 << data.flagNames.count()) - 1)
    {
        return;
    }
    if (val < 0)
    {
        return;
    }
    data.val = val;

    it.value() = data;

    const auto pfit = impl_->propertyToFlags_.constFind(property);
    auto level = 0;
    if (pfit != impl_->propertyToFlags_.constEnd())
    {
        for (auto&& prop : pfit.value())
        {
            if (prop)
                impl_->boolPropertyManager_->setValue(prop, val & (1 << level));
            level++;
        }
    }

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void FlagPropertyManager::setFlagNames(Property* property, const QStringList& flagNames)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    if (data.flagNames == flagNames)
    {
        return;
    }
    data.flagNames = flagNames;
    data.val = 0;

    it.value() = data;

    const auto pfit = impl_->propertyToFlags_.find(property);
    if (pfit != impl_->propertyToFlags_.end())
    {
        for (auto&& prop : qAsConst(pfit.value()))
        {
            if (prop)
            {
                delete prop;
                impl_->flagToProperty_.remove(prop);
            }
        }
        pfit.value().clear();
    }

    for (const auto& flagName : flagNames)
    {
        auto prop = impl_->boolPropertyManager_->addProperty();
        prop->setPropertyName(flagName);
        property->addSubProperty(prop);
        impl_->propertyToFlags_[property].append(prop);
        impl_->flagToProperty_[prop] = property;
    }

    emit flagNamesChanged(property, data.flagNames);

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void FlagPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};

    impl_->propertyToFlags_[property] = {};
}

void FlagPropertyManager::uninitializeProperty(Property* property)
{
    const auto it = impl_->propertyToFlags_.find(property);
    if (it != impl_->propertyToFlags_.end())
    {
        for (auto&& prop : qAsConst(it.value()))
        {
            if (prop)
            {
                delete prop;
                impl_->flagToProperty_.remove(prop);
            }
        }
    }
    impl_->propertyToFlags_.erase(it);

    impl_->values_.remove(property);
}
