/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "charpropertymanager.h"
#include "private/charpropertymanager_p.h"
#include "private/propertymanagerutils_p.h"

CharPropertyManager::CharPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new CharPropertyManagerPrivate)
{
    impl_->charPropertyManagerPtr_ = this;
}

CharPropertyManager::~CharPropertyManager()
{
    clear();
}

QChar CharPropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QChar());
}

QString CharPropertyManager::valueText(const Property* property) const
{
   const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto c = it.value();
    return c.isNull() ? QString() : QString(c);
}

void CharPropertyManager::setValue(Property* property, const QChar& val)
{
    setSimpleValue<const QChar&, QChar, CharPropertyManager>(impl_->values_, this,
        &CharPropertyManager::propertyChanged,
        &CharPropertyManager::valueChanged,
        property, val);
}

void CharPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = {};
}

void CharPropertyManager::uninitializeProperty(Property *property)
{
    impl_->values_.remove(property);
}
