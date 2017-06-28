/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "enumpropertymanager.h"
#include "private/enumpropertymanager_p.h"
#include "private/propertymanagerutils_p.h"
#include <QtGui/QIcon>

EnumPropertyManager::EnumPropertyManager(QObject* parent) :
    AbstractPropertyManager(parent),
    impl_(new EnumPropertyManagerPrivate)
{
    impl_->EnumPropertyManagerPtr_ = this;
}

EnumPropertyManager::~EnumPropertyManager()
{
    clear();
}

int EnumPropertyManager::value(const Property* property) const
{
    return getValue<int>(impl_->values_, property, -1);
}

QStringList EnumPropertyManager::enumNames(const Property *property) const
{
    return getData<QStringList>(impl_->values_, &EnumPropertyManagerPrivate::Data::enumNames, property, QStringList());
}

QMap<int, QIcon> EnumPropertyManager::enumIcons(const Property* property) const
{
    return getData<QMap<int, QIcon>>(impl_->values_, &EnumPropertyManagerPrivate::Data::enumIcons, property, QMap<int, QIcon>());
}

QString EnumPropertyManager::valueText(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QString();
    }
    const auto& data = it.value();

    const auto v = data.val;
    if (v >= 0 && v < data.enumNames.count())
    {
        return data.enumNames.at(v);
    }
    return QString();
}

QIcon EnumPropertyManager::valueIcon(const Property* property) const
{
    const auto it = impl_->values_.constFind(property);
    if (it == impl_->values_.constEnd())
    {
        return QIcon();
    }
    const auto& data = it.value();

    const auto v = data.val;
    return data.enumIcons.value(v);
}

void EnumPropertyManager::setValue(Property* property, int val)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    if (val >= data.enumNames.count())
    {
        return;
    }
    if (val < 0 && data.enumNames.count() > 0)
    {
        return;
    }
    if (val < 0)
    {
        val = -1;
    }
    if (data.val == val)
    {
        return;
    }
    data.val = val;

    it.value() = data;

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void EnumPropertyManager::setEnumNames(Property* property, const QStringList& enumNames)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    auto data = it.value();

    if (data.enumNames == enumNames)
    {
        return;
    }
    data.enumNames = enumNames;

    data.val = -1;

    if (enumNames.count() > 0)
    {
        data.val = 0;
    }
    it.value() = data;

    emit enumNamesChanged(property, data.enumNames);

    emit propertyChanged(property);
    emit valueChanged(property, data.val);
}

void EnumPropertyManager::setEnumIcons(Property* property, const QMap<int, QIcon>& enumIcons)
{
    const auto it = impl_->values_.find(property);
    if (it == impl_->values_.end())
    {
        return;
    }
    it.value().enumIcons = enumIcons;

    emit enumIconsChanged(property, it.value().enumIcons);

    emit propertyChanged(property);
}

void EnumPropertyManager::initializeProperty(Property* property)
{
    impl_->values_[property] = EnumPropertyManagerPrivate::Data();
}

void EnumPropertyManager::uninitializeProperty(Property* property)
{
    impl_->values_.remove(property);
}
