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
#include "propertyeditorview.h"
#include "treepropertybrowser.h"
#include "private/treepropertybrowser_p.h"
#include <QtWidgets/QHeaderView>


TreePropertyBrowser::TreePropertyBrowser(QWidget* parent) :
    AbstractPropertyBrowser(parent),
    impl_(new TreePropertyBrowserPrivate)
{
    impl_->treePropertyBrowserPtr_ = this;

    impl_->init(this);
    connect(this, &TreePropertyBrowser::currentItemChanged,
        this, [this](BrowserItem* item) { impl_->slotCurrentBrowserItemChanged(item); });
}

TreePropertyBrowser::~TreePropertyBrowser()
{
}

int TreePropertyBrowser::indentation() const
{
    return impl_->treeWidget_->indentation();
}

void TreePropertyBrowser::setIndentation(int i)
{
    impl_->treeWidget_->setIndentation(i);
}

bool TreePropertyBrowser::rootIsDecorated() const
{
    return impl_->treeWidget_->rootIsDecorated();
}

void TreePropertyBrowser::setRootIsDecorated(bool show)
{
    impl_->treeWidget_->setRootIsDecorated(show);
    for (auto it = impl_->itemToIndex_.cbegin(), end = impl_->itemToIndex_.cend(); it != end; ++it)
    {
        auto property = it.value()->property();
        if (!property->hasValue())
        {
            impl_->updateItem(it.key());
        }
    }
}

bool TreePropertyBrowser::alternatingRowColors() const
{
    return impl_->treeWidget_->alternatingRowColors();
}

void TreePropertyBrowser::setAlternatingRowColors(bool enable)
{
    impl_->treeWidget_->setAlternatingRowColors(enable);
}

bool TreePropertyBrowser::isHeaderVisible() const
{
    return impl_->headerVisible_;
}

void TreePropertyBrowser::setHeaderVisible(bool visible)
{
    if (impl_->headerVisible_ == visible)
    {
        return;
    }
    impl_->headerVisible_ = visible;
    impl_->treeWidget_->header()->setVisible(visible);
}

TreePropertyBrowser::ResizeMode TreePropertyBrowser::resizeMode() const
{
    return impl_->resizeMode_;
}

void TreePropertyBrowser::setResizeMode(TreePropertyBrowser::ResizeMode mode)
{
    if (impl_->resizeMode_ == mode)
    {
        return;
    }
    impl_->resizeMode_ = mode;
    auto m = QHeaderView::Stretch;
    switch (mode)
    {
    case TreePropertyBrowser::Interactive:
        m = QHeaderView::Interactive;
        break;
    case TreePropertyBrowser::Fixed:
        m = QHeaderView::Fixed;
        break;
    case TreePropertyBrowser::ResizeToContents:
        m = QHeaderView::ResizeToContents;
        break;
    case TreePropertyBrowser::Stretch:
    default:
        m = QHeaderView::Stretch;
        break;
    }
    impl_->treeWidget_->header()->setSectionResizeMode(m);
}

int TreePropertyBrowser::splitterPosition() const
{
    return impl_->treeWidget_->header()->sectionSize(0);
}

void TreePropertyBrowser::setSplitterPosition(int position)
{
    impl_->treeWidget_->header()->resizeSection(0, position);
}

void TreePropertyBrowser::setExpanded(BrowserItem* item, bool expanded)
{
    auto treeItem = impl_->indexToItem_.value(item);
    if (treeItem)
    {
        treeItem->setExpanded(expanded);
    }
}

bool TreePropertyBrowser::isExpanded(BrowserItem* item) const
{
    auto treeItem = impl_->indexToItem_.value(item);
    return treeItem ? treeItem->isExpanded() : false;
}

bool TreePropertyBrowser::isItemVisible(BrowserItem* item) const
{
    if (const auto treeItem = impl_->indexToItem_.value(item))
    {
        return !treeItem->isHidden();
    }
    return false;
}

void TreePropertyBrowser::setItemVisible(BrowserItem* item, bool visible)
{
    if (auto treeItem = impl_->indexToItem_.value(item))
    {
        treeItem->setHidden(!visible);
    }
}

void TreePropertyBrowser::setBackgroundColor(BrowserItem* item, const QColor& color)
{
    if (!impl_->indexToItem_.contains(item))
    {
        return;
    }
    if (color.isValid())
    {
        impl_->indexToBackgroundColor_[item] = color;
    }
    else
    {
        impl_->indexToBackgroundColor_.remove(item);
    }
    impl_->treeWidget_->viewport()->update();
}

QColor TreePropertyBrowser::backgroundColor(BrowserItem* item) const
{
    return impl_->indexToBackgroundColor_.value(item);
}

QColor TreePropertyBrowser::calculatedBackgroundColor(BrowserItem* item) const
{
    return impl_->calculatedBackgroundColor(item);
}

void TreePropertyBrowser::setPropertiesWithoutValueMarked(bool mark)
{
    if (impl_->markPropertiesWithoutValue_ == mark)
    {
        return;
    }
    impl_->markPropertiesWithoutValue_ = mark;
    for (auto it = impl_->itemToIndex_.cbegin(), end = impl_->itemToIndex_.cend(); it != end; ++it)
    {
        auto property = it.value()->property();
        if (!property->hasValue())
        {
            impl_->updateItem(it.key());
        }
    }
    impl_->treeWidget_->viewport()->update();
}

bool TreePropertyBrowser::propertiesWithoutValueMarked() const
{
    return impl_->markPropertiesWithoutValue_;
}

void TreePropertyBrowser::itemInserted(BrowserItem* item, BrowserItem* afterItem)
{
    impl_->propertyInserted(item, afterItem);
}

void TreePropertyBrowser::itemRemoved(BrowserItem* item)
{
    impl_->propertyRemoved(item);
}

void TreePropertyBrowser::itemChanged(BrowserItem* item)
{
    impl_->propertyChanged(item);
}

void TreePropertyBrowser::editItem(BrowserItem* item)
{
    impl_->editItem(item);
}
