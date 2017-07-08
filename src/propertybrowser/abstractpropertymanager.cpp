/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "abstractpropertymanager.h"
#include "property.h"
#include "private/abstractpropertymanager_p.h"
#include <QtGui/QIcon>

AbstractPropertyManager::AbstractPropertyManager(QObject* parent) :
    QObject(parent),
    impl_(new AbstractPropertyManagerPrivate)
{
    impl_->propertyManagerPtr_ = this;
}

AbstractPropertyManager::~AbstractPropertyManager()
{
    clear();
}

void AbstractPropertyManager::clear() const
{
    while (!impl_->properties_.isEmpty())
    {
        delete *impl_->properties_.cbegin();
    }
}

QSet<Property*> AbstractPropertyManager::properties() const
{
    return impl_->properties_;
}

bool AbstractPropertyManager::hasValue(const Property* property) const
{
    Q_UNUSED(property)
    return true;
}

QIcon AbstractPropertyManager::valueIcon(const Property* property) const
{
    Q_UNUSED(property)
    return QIcon();
}

QString AbstractPropertyManager::valueText(const Property* property) const
{
    Q_UNUSED(property)
    return QString();
}

Property *AbstractPropertyManager::addProperty(const QString& name)
{
    auto property = createProperty();
    if (property)
    {
        property->setPropertyName(name);
        impl_->properties_.insert(property);
        initializeProperty(property);
    }
    return property;
}

Property* AbstractPropertyManager::createProperty()
{
    return new Property(this);
}

void AbstractPropertyManager::uninitializeProperty(Property* property)
{
    Q_UNUSED(property)
}

