/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "datetimepropertymanager.h"
#include "private/datetimepropertymanager_p.h"
#include "private/propertymanagerutils_p.h"

DateTimePropertyManager::DateTimePropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new DateTimePropertyManagerPrivate(this))
{
}

DateTimePropertyManager::~DateTimePropertyManager()
{
    clear();
}

QDateTime DateTimePropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QDateTime());
}

QString DateTimePropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return it.value().toString(impl_->format_);
}

void DateTimePropertyManager::setValue(Property* property, const QDateTime& val)
{
    setSimpleValue<const QDateTime&, QDateTime, DateTimePropertyManager>(impl_->values_, this,
        &DateTimePropertyManager::propertyChanged,
        &DateTimePropertyManager::valueChanged,
        property, val);
}

void DateTimePropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = QDateTime::currentDateTime();
}

void DateTimePropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
