/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QList>

class Property;
class BrowserItem;
class AbstractPropertyBrowser;
class BrowserItemPrivate
{
public:
    BrowserItemPrivate(AbstractPropertyBrowser* browser, Property* property, BrowserItem* parent);
    BrowserItemPrivate(const BrowserItemPrivate&) = delete;
    BrowserItemPrivate& operator=(const BrowserItemPrivate&) = delete;

    void addChild(BrowserItem* index, BrowserItem* after);
    void removeChild(BrowserItem* index);

    AbstractPropertyBrowser* const browser_;
    Property* property_;
    BrowserItem* parent_;

    BrowserItem* browserItemPtr_;

    QList<BrowserItem*> children_;

};
