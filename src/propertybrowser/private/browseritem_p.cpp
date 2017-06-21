/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "browseritem_p.h"

BrowserItemPrivate::BrowserItemPrivate(AbstractPropertyBrowser* browser,
    Property* property, BrowserItem* parent) :
    browser_(browser),
    property_(property),
    parent_(parent),
    browserItemPtr_(nullptr)
{

}

void BrowserItemPrivate::addChild(BrowserItem* index, BrowserItem* after)
{
    if (children_.contains(index))
    {
        return;
    }
    auto idx = children_.indexOf(after) + 1;
    children_.insert(idx, index);
}

void BrowserItemPrivate::removeChild(BrowserItem *index)
{
    children_.removeAll(index);
}
