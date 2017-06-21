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

class ButtonPropertyBrowserPrivate;
class ButtonPropertyBrowser : public AbstractPropertyBrowser
{
    Q_OBJECT
public:

    explicit ButtonPropertyBrowser(QWidget* parent = nullptr);
    ~ButtonPropertyBrowser();

    void setExpanded(BrowserItem* item, bool expanded);
    bool isExpanded(BrowserItem* item) const;

Q_SIGNALS:

    void collapsed(BrowserItem* item);
    void expanded(BrowserItem* item);

protected:
    virtual void itemInserted(BrowserItem* item, BrowserItem* afterItem);
    virtual void itemRemoved(BrowserItem* item);
    virtual void itemChanged(BrowserItem* item);

private:
    friend class ButtonPropertyBrowserPrivate;
    QScopedPointer<ButtonPropertyBrowserPrivate> impl_;
};
