/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "timepropertymanager.h"
#include "private/timepropertymanager_p.h"
#include "private/propertymanagerutils_p.h"

TimePropertyManager::TimePropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new TimePropertyManagerPrivate(this))
{
}

TimePropertyManager::~TimePropertyManager()
{
    clear();
}

QTime TimePropertyManager::value(const Property* property) const
{
    return impl_->values_.value(property, QTime());
}

QString TimePropertyManager::valueText(const Property* property) const
{
   const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    return it.value().toString(impl_->format_);
}

void TimePropertyManager::setValue(Property* property, const QTime& val)
{
    setSimpleValue<const QTime&, QTime, TimePropertyManager>(impl_->values_, this,
        &TimePropertyManager::propertyChanged,
        &TimePropertyManager::valueChanged,
        property, val);
}

void TimePropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = QTime::currentTime();
}

void TimePropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
