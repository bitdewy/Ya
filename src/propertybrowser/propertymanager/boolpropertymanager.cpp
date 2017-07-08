/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "boolpropertymanager.h"
#include "private/boolpropertymanager_p.h"
#include "private/propertymanagerutils_p.h"

BoolPropertyManager::BoolPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new BoolPropertyManagerPrivate)
{
    impl_->boolPropertyManagerPtr_ = this;
}

BoolPropertyManager::~BoolPropertyManager()
{
    clear();
}

bool BoolPropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, false);
}

QString BoolPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    static const auto trueText = tr("True");
    static const auto falseText = tr("False");
    return it.value() ? trueText : falseText;
}

QIcon BoolPropertyManager::valueIcon(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QIcon();
    }
    return it.value() ? impl_->checkedIcon_ : impl_->uncheckedIcon_;
}

void BoolPropertyManager::setValue(Property* property, bool val)
{
    setSimpleValue<bool, bool, BoolPropertyManager>(impl_->values_, this,
        &BoolPropertyManager::propertyChanged,
        &BoolPropertyManager::valueChanged,
        property, val);
}

void BoolPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = false;
}

void BoolPropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
