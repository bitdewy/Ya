/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "property.h"
#include "browseritem.h"
#include "groupboxpropertybrowser.h"
#include "private/groupboxpropertybrowser_p.h"

GroupBoxPropertyBrowser::GroupBoxPropertyBrowser(QWidget* parent) :
    AbstractPropertyBrowser(parent),
    impl_(new GroupBoxPropertyBrowserPrivate)
{
    impl_->groupBoxPropertyBrowserPtr_ = this;

    impl_->init(this);
}

GroupBoxPropertyBrowser::~GroupBoxPropertyBrowser()
{
    const auto icend = impl_->itemToIndex_.constEnd();
    for (auto it = impl_->itemToIndex_.constBegin(); it != icend; ++it)
    {
        delete it.key();
    }
}

void GroupBoxPropertyBrowser::itemInserted(BrowserItem* item, BrowserItem* afterItem)
{
    impl_->propertyInserted(item, afterItem);
}

void GroupBoxPropertyBrowser::itemRemoved(BrowserItem* item)
{
    impl_->propertyRemoved(item);
}

void GroupBoxPropertyBrowser::itemChanged(BrowserItem* item)
{
    impl_->propertyChanged(item);
}
