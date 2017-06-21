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
#include <QtCore/QObject>

class Property;
class AbstractPropertyBrowser;
class BrowserItemPrivate;
class BrowserItem
{
public:
    Property* property() const;
    BrowserItem* parent() const;
    QList<BrowserItem*> children() const;
    AbstractPropertyBrowser* browser() const;
private:
    explicit BrowserItem(AbstractPropertyBrowser* browser, Property* property, BrowserItem* parent);
    ~BrowserItem();

    BrowserItem(const BrowserItem&) = delete;
    BrowserItem& operator=(const BrowserItem&) = delete;

    QScopedPointer<BrowserItemPrivate> impl_;
    friend class AbstractPropertyBrowserPrivate;
};
