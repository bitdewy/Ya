/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "stringpropertymanager.h"
#include "private/stringpropertymanager_p.h"
#include "private/propertymanagerutils_p.h"

StringPropertyManager::StringPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new StringPropertyManagerPrivate)
{
    impl_->stringPropertyManagerPtr_ = this;
}

StringPropertyManager::~StringPropertyManager()
{
    clear();
}

QString StringPropertyManager::value(const Property* property) const
{
    return getValue<QString>(impl_->values_, property);
}

QRegExp StringPropertyManager::regExp(const Property* property) const
{
    return getData<QRegExp>(impl_->values_, &StringPropertyManagerPrivate::Data::regExp, property, QRegExp());
}

QString StringPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    return it == impl_->values_.constEnd() ? QString() : it.value().val;
}

void StringPropertyManager::setValue(Property* property, const QString& val)
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
    if (data.regExp.isValid() && !data.regExp.exactMatch(val))
    {
        return;
    }
    data.val = val;

    it.value() = data;

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void StringPropertyManager::setRegExp(Property* property, const QRegExp& regExp)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value() ;

    if (data.regExp == regExp)
    {
        return;
    }
    data.regExp = regExp;

    it.value() = data;

    emit regExpChanged(property, data.regExp);
}

void StringPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};
}

void StringPropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
