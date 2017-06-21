/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "abstractpropertybrowser.h"
#include "private/abstractpropertybrowser_p.h"

AbstractPropertyBrowser::AbstractPropertyBrowser(QWidget *parent) :
    QWidget(parent),
    impl_(new AbstractPropertyBrowserPrivate)
{
    impl_->abstractPropertyBrowserPtr_ = this;

}

AbstractPropertyBrowser::~AbstractPropertyBrowser()
{
    const auto indexes = topLevelItems();
    for (auto&& item : indexes)
    {
        impl_->clearIndex(item);
    }
}

QList<Property*> AbstractPropertyBrowser::properties() const
{
    return impl_->subItems_;
}

QList<BrowserItem*> AbstractPropertyBrowser::items(Property* property) const
{
    return impl_->propertyToIndexes_.value(property);
}

BrowserItem *AbstractPropertyBrowser::topLevelItem(Property* property) const
{
    return impl_->topLevelPropertyToIndex_.value(property);
}

QList<BrowserItem*> AbstractPropertyBrowser::topLevelItems() const
{
    return impl_->topLevelIndexes_;
}

void AbstractPropertyBrowser::clear()
{
    const auto subList = properties();
    for (auto rit = subList.crbegin(), rend = subList.crend(); rit != rend; ++rit)
    {
        removeProperty(*rit);
    }
}

BrowserItem* AbstractPropertyBrowser::addProperty(Property* property)
{
    Property* afterProperty = nullptr;
    if (impl_->subItems_.count() > 0)
    {
        afterProperty = impl_->subItems_.last();
    }
    return insertProperty(property, afterProperty);
}

BrowserItem* AbstractPropertyBrowser::insertProperty(Property* property,
        Property *afterProperty)
{
    if (!property)
    {
        return nullptr;
    }
    auto pendingList = properties();
    int pos = 0;
    int newPos = 0;
    while (pos < pendingList.count())
    {
        auto prop = pendingList.at(pos);
        if (prop == property)
        {
            return 0;
        }
        if (prop == afterProperty)
        {
            newPos = pos + 1;
        }
        pos++;
    }
    impl_->createBrowserIndexes(property, 0, afterProperty);
    impl_->insertSubTree(property, 0);
    impl_->subItems_.insert(newPos, property);

    return topLevelItem(property);
}

void AbstractPropertyBrowser::removeProperty(Property* property)
{
    if (!property)
    {
        return;
    }
    auto pendingList = properties();
    int pos = 0;
    while (pos < pendingList.count())
    {
        if (pendingList.at(pos) == property)
        {
            impl_->subItems_.removeAt(pos);
            impl_->removeSubTree(property, 0);

            impl_->removeBrowserIndexes(property, 0);

            return;
        }
        pos++;
    }
}

QWidget *AbstractPropertyBrowser::createEditor(Property* property,
        QWidget* parent)
{
    AbstractEditorFactoryBase* factory = nullptr;
    auto manager = property->propertyManager();

    if (viewToManagerToFactory()->contains(this) &&
        (*viewToManagerToFactory())[this].contains(manager))
    {
        factory = (*viewToManagerToFactory())[this][manager];
    }

    if (!factory)
    {
        return nullptr;
    }
    auto w = factory->createEditor(property, parent);
    if (w)
    {
        w->setFocusPolicy(Qt::WheelFocus);
    }
    return w;
}

void AbstractPropertyBrowser::slotPropertyInserted(Property *property,
        Property* parentProperty, Property* afterProperty)
{
    impl_->slotPropertyInserted(property, parentProperty, afterProperty);
}

void AbstractPropertyBrowser::slotPropertyRemoved(Property* property,
        Property *parentProperty)
{
    impl_->slotPropertyRemoved(property, parentProperty);
}

void AbstractPropertyBrowser::slotPropertyDestroyed(Property* property)
{
    impl_->slotPropertyDestroyed(property);
}

void AbstractPropertyBrowser::slotPropertyDataChanged(Property* property)
{
    impl_->slotPropertyDataChanged(property);
}

bool AbstractPropertyBrowser::addFactory(AbstractPropertyManager* abstractManager,
        AbstractEditorFactoryBase* abstractFactory)
{
    bool connectNeeded = false;
    if (!managerToFactoryToViews()->contains(abstractManager) ||
        !(*managerToFactoryToViews())[abstractManager].contains(abstractFactory))
    {
        connectNeeded = true;
    }
    else if ((*managerToFactoryToViews())[abstractManager][abstractFactory].contains(this))
    {
        return connectNeeded;
    }

    if (viewToManagerToFactory()->contains(this) &&
        (*viewToManagerToFactory())[this].contains(abstractManager))
    {
        unsetFactoryForManager(abstractManager);
    }

    (*managerToFactoryToViews())[abstractManager][abstractFactory].append(this);
    (*viewToManagerToFactory())[this][abstractManager] = abstractFactory;

    return connectNeeded;
}

void AbstractPropertyBrowser::unsetFactoryForManager(AbstractPropertyManager* manager)
{
    if (!viewToManagerToFactory()->contains(this) ||
        !(*viewToManagerToFactory())[this].contains(manager))
    {
        return;
    }

    auto abstractFactory = (*viewToManagerToFactory())[this][manager];
    (*viewToManagerToFactory())[this].remove(manager);
    if ((*viewToManagerToFactory())[this].isEmpty())
    {
        (*viewToManagerToFactory()).remove(this);
    }

    (*managerToFactoryToViews())[manager][abstractFactory].removeAll(this);
    if ((*managerToFactoryToViews())[manager][abstractFactory].isEmpty())
    {
        (*managerToFactoryToViews())[manager].remove(abstractFactory);
        abstractFactory->breakConnection(manager);
        if ((*managerToFactoryToViews())[manager].isEmpty())
        {
            (*managerToFactoryToViews()).remove(manager);
        }
    }
}

BrowserItem* AbstractPropertyBrowser::currentItem() const
{
    return impl_->currentItem_;
}

void AbstractPropertyBrowser::setCurrentItem(BrowserItem* item)
{
    auto oldItem = impl_->currentItem_;
    impl_->currentItem_ = item;
    if (oldItem != item)
    {
        emit currentItemChanged(item);
    }
}
