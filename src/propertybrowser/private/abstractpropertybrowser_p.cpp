/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "abstractpropertybrowser.h"
#include "abstractpropertybrowser_p.h"
#include "abstracteditorfactorybase.h"
#include "abstractpropertybrowser.h"
#include "abstractpropertymanager.h"
#include "browseritem.h"
#include "private/browseritem_p.h"
#include "property.h"

AbstractPropertyBrowserPrivate::AbstractPropertyBrowserPrivate() :
   currentItem_(nullptr)
{

}

void AbstractPropertyBrowserPrivate::insertSubTree(Property* property,
        Property* parentProperty)
{
    if (propertyToParents_.contains(property))
    {
        propertyToParents_[property].append(parentProperty);
        return;
    }
    auto manager = property->propertyManager();
    if (managerToProperties_[manager].isEmpty())
    {
        abstractPropertyBrowserPtr_->connect(manager, &AbstractPropertyManager::propertyInserted,
                       abstractPropertyBrowserPtr_, &AbstractPropertyBrowser::slotPropertyInserted);
        abstractPropertyBrowserPtr_->connect(manager, &AbstractPropertyManager::propertyRemoved,
                       abstractPropertyBrowserPtr_, &AbstractPropertyBrowser::slotPropertyRemoved);
        abstractPropertyBrowserPtr_->connect(manager, &AbstractPropertyManager::propertyDestroyed,
                       abstractPropertyBrowserPtr_, &AbstractPropertyBrowser::slotPropertyDestroyed);
        abstractPropertyBrowserPtr_->connect(manager, &AbstractPropertyManager::propertyChanged,
                       abstractPropertyBrowserPtr_, &AbstractPropertyBrowser::slotPropertyDataChanged);
    }
    managerToProperties_[manager].append(property);
    propertyToParents_[property].append(parentProperty);

    const auto subList = property->subProperties();
    for (auto&& subProperty : subList)
    {
        insertSubTree(subProperty, property);
    }
}

void AbstractPropertyBrowserPrivate::removeSubTree(Property* property,
        Property* parentProperty)
{
    if (!propertyToParents_.contains(property))
    {
        return;
    }

    propertyToParents_[property].removeAll(parentProperty);
    if (!propertyToParents_[property].isEmpty())
    {
        return;
    }
    propertyToParents_.remove(property);
    auto manager = property->propertyManager();
    managerToProperties_[manager].removeAll(property);
    if (managerToProperties_[manager].isEmpty())
    {
        abstractPropertyBrowserPtr_->disconnect(manager, &AbstractPropertyManager::propertyInserted,
                abstractPropertyBrowserPtr_, &AbstractPropertyBrowser::slotPropertyInserted);
        abstractPropertyBrowserPtr_->disconnect(manager, &AbstractPropertyManager::propertyRemoved,
                abstractPropertyBrowserPtr_, &AbstractPropertyBrowser::slotPropertyRemoved);
        abstractPropertyBrowserPtr_->disconnect(manager, &AbstractPropertyManager::propertyDestroyed,
                abstractPropertyBrowserPtr_, &AbstractPropertyBrowser::slotPropertyDestroyed);
        abstractPropertyBrowserPtr_->disconnect(manager, &AbstractPropertyManager::propertyChanged,
                abstractPropertyBrowserPtr_, &AbstractPropertyBrowser::slotPropertyDataChanged);

        managerToProperties_.remove(manager);
    }

    const auto subList = property->subProperties();
    for (auto&& subProperty : subList)
    {
        removeSubTree(subProperty, property);
    }
}

void AbstractPropertyBrowserPrivate::createBrowserIndexes(Property* property,
    Property* parentProperty, Property* afterProperty)
{
    QMap<BrowserItem*, BrowserItem*> parentToAfter;
    if (afterProperty)
    {
        auto it = propertyToIndexes_.constFind(afterProperty);
        if (it == propertyToIndexes_.constEnd())
        {
            return;
        }
        for (auto&& idx : it.value())
        {
            auto parentIdx = idx->parent();
            if ((parentProperty && parentIdx && parentIdx->property() == parentProperty) ||
                    (!parentProperty && !parentIdx))
            {
                parentToAfter[idx->parent()] = idx;
            }
        }
    }
    else if (parentProperty)
    {
        auto it = propertyToIndexes_.find(parentProperty);
        if (it == propertyToIndexes_.constEnd())
        {
            return;
        }
        for (auto&& idx : it.value())
        {
            parentToAfter[idx] = 0;
        }
    }
    else
    {
        parentToAfter[0] = 0;
    }

    const auto pcend = parentToAfter.constEnd();
    for (auto it = parentToAfter.constBegin(); it != pcend; ++it)
    {
        createBrowserIndex(property, it.key(), it.value());
    }
}

BrowserItem *AbstractPropertyBrowserPrivate::createBrowserIndex(Property* property,
        BrowserItem* parentIndex, BrowserItem* afterIndex)
{
    auto newIndex = new BrowserItem(abstractPropertyBrowserPtr_, property, parentIndex);
    if (parentIndex)
    {
        parentIndex->impl_->addChild(newIndex, afterIndex);
    }
    else
    {
        topLevelPropertyToIndex_[property] = newIndex;
        topLevelIndexes_.insert(topLevelIndexes_.indexOf(afterIndex) + 1, newIndex);
    }
    propertyToIndexes_[property].append(newIndex);

    abstractPropertyBrowserPtr_->itemInserted(newIndex, afterIndex);

    const auto subItems = property->subProperties();
    BrowserItem* afterChild = nullptr;
    for (auto&& child : subItems)
    {
        afterChild = createBrowserIndex(child, newIndex, afterChild);
    }
    return newIndex;
}

void AbstractPropertyBrowserPrivate::removeBrowserIndexes(Property* property,
        Property* parentProperty)
{
    QList<BrowserItem *> toRemove;
    auto it = propertyToIndexes_.constFind(property);
    if (it == propertyToIndexes_.constEnd())
    {
        return;
    }
    for (auto&& idx : it.value())
    {
        auto parentIdx = idx->parent();
        if ((parentProperty && parentIdx && parentIdx->property() == parentProperty) ||
                (!parentProperty && !parentIdx))
        {
            toRemove.append(idx);
        }
    }

    for (const auto& index : qAsConst(toRemove))
    {
        removeBrowserIndex(index);
    }
}

void AbstractPropertyBrowserPrivate::removeBrowserIndex(BrowserItem* index)
{
    QList<BrowserItem *> children = index->children();
    for (int i = children.count(); i > 0; i--)
    {
        removeBrowserIndex(children.at(i - 1));
    }

    abstractPropertyBrowserPtr_->itemRemoved(index);

    if (index->parent())
    {
        index->parent()->impl_->removeChild(index);
    }
    else
    {
        topLevelPropertyToIndex_.remove(index->property());
        topLevelIndexes_.removeAll(index);
    }

    auto property = index->property();

    propertyToIndexes_[property].removeAll(index);
    if (propertyToIndexes_[property].isEmpty())
    {
        propertyToIndexes_.remove(property);
    }
    delete index;
}

void AbstractPropertyBrowserPrivate::clearIndex(BrowserItem* index)
{
    const auto children = index->children();
    for (auto&& item : children)
    {
        clearIndex(item);
    }
    delete index;
}

void AbstractPropertyBrowserPrivate::slotPropertyInserted(Property* property,
        Property* parentProperty, Property* afterProperty)
{
    if (!propertyToParents_.contains(parentProperty))
    {
        return;
    }
    createBrowserIndexes(property, parentProperty, afterProperty);
    insertSubTree(property, parentProperty);
}

void AbstractPropertyBrowserPrivate::slotPropertyRemoved(Property* property,
        Property* parentProperty)
{
    if (!propertyToParents_.contains(parentProperty))
    {
        return;
    }
    removeSubTree(property, parentProperty);
    removeBrowserIndexes(property, parentProperty);
}

void AbstractPropertyBrowserPrivate::slotPropertyDestroyed(Property* property)
{
    if (!subItems_.contains(property))
    {
        return;
    }
    abstractPropertyBrowserPtr_->removeProperty(property);
}

void AbstractPropertyBrowserPrivate::slotPropertyDataChanged(Property* property)
{
    if (!propertyToParents_.contains(property))
    {
        return;
    }
    auto it = propertyToIndexes_.find(property);
    if (it == propertyToIndexes_.constEnd())
    {
        return;
    }
    const auto indexes = it.value();
    for (auto&& idx : indexes)
    {
        abstractPropertyBrowserPtr_->itemChanged(idx);
    }
}
