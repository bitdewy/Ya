/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "property.h"
#include "abstractpropertymanager.h"
#include "private/abstractpropertymanager_p.h"
#include "private/property_p.h"
#include <QtCore/QSet>
#include <QtCore/QMap>

Property::Property(AbstractPropertyManager* manager)
    : impl_(new PropertyPrivate(manager))
{
    impl_->propertyPtr_ = this;
}

Property::~Property()
{
    for (const auto& property : qAsConst(impl_->parentItems_))
    {
        property->impl_->manager_->impl_->propertyRemoved(this, property);
    }

    impl_->manager_->impl_->propertyDestroyed(this);

    for (const auto& property : qAsConst(impl_->subItems_))
    {
        property->impl_->parentItems_.remove(this);
    }

    for (const auto& property : qAsConst(impl_->parentItems_))
    {
        property->impl_->subItems_.removeAll(this);
    }
}

QList<Property*> Property::subProperties() const
{
    return impl_->subItems_;
}

AbstractPropertyManager* Property::propertyManager() const
{
    return impl_->manager_;
}

QString Property::toolTip() const
{
    return impl_->toolTip_;
}

QString Property::descriptionToolTip() const
{
    return impl_->descriptionToolTip_;
}

QString Property::statusTip() const
{
    return impl_->statusTip_;
}

QString Property::whatsThis() const
{
    return impl_->whatsThis_;
}

QString Property::propertyName() const
{
    return impl_->name_;
}

bool Property::isEnabled() const
{
    return impl_->enabled_;
}

bool Property::isModified() const
{
    return impl_->modified_;
}

bool Property::hasValue() const
{
    return impl_->manager_->hasValue(this);
}

QIcon Property::valueIcon() const
{
    return impl_->manager_->valueIcon(this);
}

QString Property::valueText() const
{
    return impl_->manager_->valueText(this);
}

void Property::setToolTip(const QString& text)
{
    if (impl_->toolTip_ == text)
    {
        return;
    }
    impl_->toolTip_ = text;
    propertyChanged();
}

void Property::setDescriptionToolTip(const QString& text)
{
    if (impl_->descriptionToolTip_ == text)
    {
        return;
    }
    impl_->descriptionToolTip_ = text;
    propertyChanged();
}

void Property::setStatusTip(const QString& text)
{
    if (impl_->statusTip_ == text)
    {
        return;
    }
    impl_->statusTip_ = text;
    propertyChanged();
}

void Property::setWhatsThis(const QString& text)
{
    if (impl_->whatsThis_ == text)
    {
        return;
    }
    impl_->whatsThis_ = text;
    propertyChanged();
}

void Property::setPropertyName(const QString& text)
{
    if (impl_->name_ == text)
    {
        return;
    }
    impl_->name_ = text;
    propertyChanged();
}

void Property::setEnabled(bool enable)
{
    if (impl_->enabled_ == enable)
    {
        return;
    }
    impl_->enabled_ = enable;
    propertyChanged();
}

void Property::setModified(bool modified)
{
    if (impl_->modified_ == modified)
    {
        return;
    }
    impl_->modified_ = modified;
    propertyChanged();
}

void Property::addSubProperty(Property* property)
{
    Property* after = nullptr;
    if (impl_->subItems_.count() > 0)
    {
        after = impl_->subItems_.last();
    }
    insertSubProperty(property, after);
}

void Property::insertSubProperty(Property* property, Property* afterProperty)
{
    if (!property)
    {
        return;
    }
    if (property == this)
    {
        return;
    }

    auto pendingList = property->subProperties();
    QMap<Property*, bool> visited;
    while (!pendingList.isEmpty())
    {
        auto i = pendingList.first();
        if (i == this)
        {
            return;
        }
        pendingList.removeFirst();
        if (visited.contains(i))
        {
            continue;
        }
        visited[i] = true;
        pendingList += i->subProperties();
    }

    pendingList = subProperties();
    int pos = 0;
    int newPos = 0;
    Property* properAfterProperty = nullptr;
    while (pos < pendingList.count())
    {
        auto i = pendingList.at(pos);
        if (i == property)
        {
            return; // already inserted
        }
        if (i == afterProperty)
        {
            newPos = pos + 1;
            properAfterProperty = afterProperty;
        }
        pos++;
    }

    impl_->subItems_.insert(newPos, property);
    property->impl_->parentItems_.insert(this);

    impl_->manager_->impl_->propertyInserted(property, this, properAfterProperty);
}

void Property::removeSubProperty(Property* property)
{
    if (!property)
    {
        return;
    }
    impl_->manager_->impl_->propertyRemoved(property, this);

    auto pendingList = subProperties();
    int pos = 0;
    while (pos < pendingList.count())
    {
        if (pendingList.at(pos) == property)
        {
            impl_->subItems_.removeAt(pos);
            property->impl_->parentItems_.remove(this);

            return;
        }
        pos++;
    }
}

void Property::propertyChanged()
{
    impl_->manager_->impl_->propertyChanged(this);
}
