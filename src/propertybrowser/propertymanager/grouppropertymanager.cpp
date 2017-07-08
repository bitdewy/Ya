/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "propertymanager/grouppropertymanager.h"

GroupPropertyManager::GroupPropertyManager(QObject* parent)
    : AbstractPropertyManager(parent)
{

}

GroupPropertyManager::~GroupPropertyManager()
{

}

bool GroupPropertyManager::hasValue(const Property* property) const
{
    Q_UNUSED(property)
    return false;
}

void GroupPropertyManager::initializeProperty(Property* property)
{
    Q_UNUSED(property)
}

void GroupPropertyManager::uninitializeProperty(Property* property)
{
    Q_UNUSED(property)
}
