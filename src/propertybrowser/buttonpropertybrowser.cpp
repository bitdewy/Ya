/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "property.h"
#include "buttonpropertybrowser.h"
#include "private/buttonpropertybrowser_p.h"
#include <QtCore/QSet>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>
#include <QtCore/QMap>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QStyle>

ButtonPropertyBrowser::ButtonPropertyBrowser(QWidget *parent) :
    AbstractPropertyBrowser(parent),
    impl_(new ButtonPropertyBrowserPrivate)
{
    impl_->buttonPropertyBrowserPtr_ = this;

    impl_->init(this);
}

ButtonPropertyBrowser::~ButtonPropertyBrowser()
{
    const auto icend = impl_->itemToIndex_.constEnd();
    for (auto it =  impl_->itemToIndex_.constBegin(); it != icend; ++it)
    {
        delete it.key();
    }
}

void ButtonPropertyBrowser::itemInserted(BrowserItem* item, BrowserItem* afterItem)
{
    impl_->propertyInserted(item, afterItem);
}

void ButtonPropertyBrowser::itemRemoved(BrowserItem* item)
{
    impl_->propertyRemoved(item);
}

void ButtonPropertyBrowser::itemChanged(BrowserItem* item)
{
    impl_->propertyChanged(item);
}

void ButtonPropertyBrowser::setExpanded(BrowserItem* item, bool expanded)
{
    auto itm = impl_->indexToItem_.value(item);
    if (itm)
    {
        impl_->setExpanded(itm, expanded);
    }
}

bool ButtonPropertyBrowser::isExpanded(BrowserItem* item) const
{
    auto itm = impl_->indexToItem_.value(item);
    return itm ? itm->expanded : false;
}
