/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "browseritem.h"
#include "private/browseritem_p.h"

Property* BrowserItem::property() const
{
    return impl_->property_;
}

BrowserItem* BrowserItem::parent() const
{
    return impl_->parent_;
}

QList<BrowserItem*> BrowserItem::children() const
{
    return impl_->children_;
}

AbstractPropertyBrowser* BrowserItem::browser() const
{
    return impl_->browser_;
}

BrowserItem::BrowserItem(AbstractPropertyBrowser* browser, Property* property, BrowserItem* parent) :
    impl_(new BrowserItemPrivate(browser, property, parent))
{
    impl_->browserItemPtr_ = this;
}

BrowserItem::~BrowserItem()
{

}
