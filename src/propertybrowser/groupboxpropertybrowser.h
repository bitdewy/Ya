/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstractpropertybrowser.h"

class GroupBoxPropertyBrowserPrivate;

class GroupBoxPropertyBrowser : public AbstractPropertyBrowser
{
    Q_OBJECT
public:
    explicit GroupBoxPropertyBrowser(QWidget* parent = nullptr);
    ~GroupBoxPropertyBrowser();

protected:
    virtual void itemInserted(BrowserItem* item, BrowserItem* afterItem);
    virtual void itemRemoved(BrowserItem* item);
    virtual void itemChanged(BrowserItem* item);

private:
    QScopedPointer<GroupBoxPropertyBrowserPrivate> impl_;
    friend class GroupBoxPropertyBrowserPrivate;
};
