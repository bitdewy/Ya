/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "abstractpropertymanager_p.h"
#include "abstractpropertymanager.h"

AbstractPropertyManagerPrivate::AbstractPropertyManagerPrivate()
{

}

AbstractPropertyManagerPrivate::~AbstractPropertyManagerPrivate()
{

}

void AbstractPropertyManagerPrivate::propertyDestroyed(Property* property)
{
    if (properties_.contains(property))
    {
        emit propertyManagerPtr_->propertyDestroyed(property);
        propertyManagerPtr_->uninitializeProperty(property);
        properties_.remove(property);
    }
}

void AbstractPropertyManagerPrivate::propertyChanged(Property* property) const
{
    emit propertyManagerPtr_->propertyChanged(property);
}

void AbstractPropertyManagerPrivate::propertyRemoved(Property* property, Property* parentProperty) const
{
    emit propertyManagerPtr_->propertyRemoved(property, parentProperty);
}

void AbstractPropertyManagerPrivate::propertyInserted(Property* property, Property* parentProperty, Property* afterProperty) const
{
    emit propertyManagerPtr_->propertyInserted(property, parentProperty, afterProperty);
}
