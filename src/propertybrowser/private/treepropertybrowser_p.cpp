/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "treepropertybrowser_p.h"
#include "propertyeditorview.h"
#include "propertyeditordelegate.h"
#include "browseritem.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>

static QIcon drawIndicatorIcon(const QPalette& palette, QStyle* style)
{
    QPixmap pix(14, 14);
    pix.fill(Qt::transparent);
    QStyleOption branchOption;
    QRect r(QPoint(0, 0), pix.size());
    branchOption.rect = QRect(2, 2, 9, 9);
    branchOption.palette = palette;
    branchOption.state = QStyle::State_Children;

    QPainter p;

    p.begin(&pix);
    style->drawPrimitive(QStyle::PE_IndicatorBranch, &branchOption, &p);
    p.end();
    QIcon rc = pix;
    rc.addPixmap(pix, QIcon::Selected, QIcon::Off);

    branchOption.state |= QStyle::State_Open;
    pix.fill(Qt::transparent);
    p.begin(&pix);
    style->drawPrimitive(QStyle::PE_IndicatorBranch, &branchOption, &p);
    p.end();

    rc.addPixmap(pix, QIcon::Normal, QIcon::On);
    rc.addPixmap(pix, QIcon::Selected, QIcon::On);
    return rc;
}

TreePropertyBrowserPrivate::TreePropertyBrowserPrivate() :
    treeWidget_(nullptr),
    headerVisible_(true),
    resizeMode_(TreePropertyBrowser::Stretch),
    delegate_(nullptr),
    markPropertiesWithoutValue_(false),
    browserChangedBlocked_(false)
{
}

void TreePropertyBrowserPrivate::init(QWidget* parent)
{
    auto layout = new QHBoxLayout(parent);
    layout->setMargin(0);
    treeWidget_ = new PropertyEditorView(parent);
    treeWidget_->setEditorPrivate(this);
    treeWidget_->setIconSize(QSize(18, 18));
    layout->addWidget(treeWidget_);

    treeWidget_->setColumnCount(2);
    QStringList labels;
    labels.append(QCoreApplication::translate("QtTreePropertyBrowser", "Property"));
    labels.append(QCoreApplication::translate("QtTreePropertyBrowser", "Value"));
    treeWidget_->setHeaderLabels(labels);
    treeWidget_->setAlternatingRowColors(true);
    treeWidget_->setEditTriggers(QAbstractItemView::EditKeyPressed);
    delegate_ = new PropertyEditorDelegate(parent);
    delegate_->setEditorPrivate(this);
    treeWidget_->setItemDelegate(delegate_);
    treeWidget_->header()->setSectionsMovable(false);
    treeWidget_->header()->setSectionResizeMode(QHeaderView::Stretch);

    expandIcon_ = drawIndicatorIcon(treePropertyBrowserPtr_->palette(), treePropertyBrowserPtr_->style());

    QObject::connect(treeWidget_, &PropertyEditorView::collapsed,
        treePropertyBrowserPtr_, [this](const QModelIndex& index) { slotCollapsed(index); });
    QObject::connect(treeWidget_, &PropertyEditorView::expanded,
        treePropertyBrowserPtr_, [this](const QModelIndex& index) { slotExpanded(index); });
    QObject::connect(treeWidget_, &PropertyEditorView::currentItemChanged,
        treePropertyBrowserPtr_, [this](QTreeWidgetItem* current, QTreeWidgetItem* previous) { slotCurrentTreeItemChanged(current, previous); });
}

BrowserItem* TreePropertyBrowserPrivate::currentItem() const
{
    if (auto treeItem = treeWidget_->currentItem())
    {
        return itemToIndex_.value(treeItem);
    }
    return nullptr;
}

void TreePropertyBrowserPrivate::setCurrentItem(BrowserItem *browserItem, bool block)
{
    const auto blocked = block ? treeWidget_->blockSignals(true) : false;
    if (!browserItem)
    {
        treeWidget_->setCurrentItem(0);
    }
    else
    {
        treeWidget_->setCurrentItem(indexToItem_.value(browserItem));
    }
    if (block)
    {
        treeWidget_->blockSignals(blocked);
    }
}

Property* TreePropertyBrowserPrivate::indexToProperty(const QModelIndex& index) const
{
    auto item = treeWidget_->indexToItem(index);
    auto idx = itemToIndex_.value(item);
    return idx ? idx->property() : nullptr;
}

BrowserItem* TreePropertyBrowserPrivate::indexToBrowserItem(const QModelIndex& index) const
{
    auto item = treeWidget_->indexToItem(index);
    return itemToIndex_.value(item);
}

QTreeWidgetItem* TreePropertyBrowserPrivate::indexToItem(const QModelIndex& index) const
{
    return treeWidget_->indexToItem(index);
}

bool TreePropertyBrowserPrivate::lastColumn(int column) const
{
    return treeWidget_->header()->visualIndex(column) == treeWidget_->columnCount() - 1;
}

void TreePropertyBrowserPrivate::disableItem(QTreeWidgetItem* item) const
{
    auto flags = item->flags();
    if (flags & Qt::ItemIsEnabled)
    {
        flags &= ~Qt::ItemIsEnabled;
        item->setFlags(flags);
        delegate_->closeEditor(itemToIndex_[item]->property());
        const auto childCount = item->childCount();
        for (auto i = 0; i < childCount; i++)
        {
            auto child = item->child(i);
            disableItem(child);
        }
    }
}

void TreePropertyBrowserPrivate::enableItem(QTreeWidgetItem* item) const
{
    auto flags = item->flags();
    flags |= Qt::ItemIsEnabled;
    item->setFlags(flags);
    const auto childCount = item->childCount();
    for (auto i = 0; i < childCount; i++)
    {
        auto child = item->child(i);
        auto property = itemToIndex_[child]->property();
        if (property->isEnabled())
        {
            enableItem(child);
        }
    }
}

bool TreePropertyBrowserPrivate::hasValue(QTreeWidgetItem* item) const
{
    auto browserItem = itemToIndex_.value(item);
    return browserItem ? browserItem->property()->hasValue() : false;
}

void TreePropertyBrowserPrivate::propertyInserted(BrowserItem* index, BrowserItem* afterIndex)
{
    auto afterItem = indexToItem_.value(afterIndex);
    auto parentItem = indexToItem_.value(index->parent());

    QTreeWidgetItem* newItem = nullptr;
    if (parentItem)
    {
        newItem = new QTreeWidgetItem(parentItem, afterItem);
    }
    else
    {
        newItem = new QTreeWidgetItem(treeWidget_, afterItem);
    }
    itemToIndex_[newItem] = index;
    indexToItem_[index] = newItem;

    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    treeWidget_->setItemExpanded(newItem, true);

    updateItem(newItem);
}

void TreePropertyBrowserPrivate::propertyRemoved(BrowserItem* index)
{
    auto item = indexToItem_.value(index);

    if (treeWidget_->currentItem() == item)
    {
        treeWidget_->setCurrentItem(0);
    }

    delete item;

    indexToItem_.remove(index);
    itemToIndex_.remove(item);
    indexToBackgroundColor_.remove(index);
}

void TreePropertyBrowserPrivate::propertyChanged(BrowserItem* index)
{
    auto item = indexToItem_.value(index);
    updateItem(item);
}

QWidget* TreePropertyBrowserPrivate::createEditor(Property* property, QWidget* parent) const
{
    return treePropertyBrowserPtr_->createEditor(property, parent);
}

void TreePropertyBrowserPrivate::updateItem(QTreeWidgetItem* item)
{
    auto property = itemToIndex_[item]->property();
    QIcon expandIcon;
    if (property->hasValue())
    {
        const auto toolTip = property->toolTip();
        const auto valueText = property->valueText();
        item->setToolTip(1, toolTip.isEmpty() ? valueText : toolTip);
        item->setIcon(1, property->valueIcon());
        item->setText(1, valueText);
    }
    else if (markPropertiesWithoutValue() && !treeWidget_->rootIsDecorated())
    {
        expandIcon = expandIcon_;
    }
    item->setIcon(0, expandIcon);
    item->setFirstColumnSpanned(!property->hasValue());
    const auto descriptionToolTip  = property->descriptionToolTip();
    const auto propertyName = property->propertyName();
    item->setToolTip(0, descriptionToolTip.isEmpty() ? propertyName : descriptionToolTip);
    item->setStatusTip(0, property->statusTip());
    item->setWhatsThis(0, property->whatsThis());
    item->setText(0, propertyName);
    bool wasEnabled = item->flags() & Qt::ItemIsEnabled;
    auto isEnabled = wasEnabled;
    if (property->isEnabled())
    {
        auto parent = item->parent();
        if (!parent || (parent->flags() & Qt::ItemIsEnabled))
        {
            isEnabled = true;
        }
        else
        {
            isEnabled = false;
        }
    }
    else
    {
        isEnabled = false;
    }
    if (wasEnabled != isEnabled)
    {
        if (isEnabled)
        {
            enableItem(item);
        }
        else
        {
            disableItem(item);
        }
    }
    treeWidget_->viewport()->update();
}

QColor TreePropertyBrowserPrivate::calculatedBackgroundColor(BrowserItem* item) const
{
    auto i = item;
    const auto itEnd = indexToBackgroundColor_.constEnd();
    while (i)
    {
        auto it = indexToBackgroundColor_.constFind(i);
        if (it != itEnd)
        {
            return it.value();
        }
        i = i->parent();
    }
    return QColor();
}

PropertyEditorView *TreePropertyBrowserPrivate::treeWidget() const
{
    return treeWidget_;
}

bool TreePropertyBrowserPrivate::markPropertiesWithoutValue() const
{
     return markPropertiesWithoutValue_;
}

void TreePropertyBrowserPrivate::slotCollapsed(const QModelIndex& index)
{
    auto item = indexToItem(index);
    auto idx = itemToIndex_.value(item);
    if (item)
    {
        emit treePropertyBrowserPtr_->collapsed(idx);
    }
}

void TreePropertyBrowserPrivate::slotExpanded(const QModelIndex& index)
{
    auto item = indexToItem(index);
    auto idx = itemToIndex_.value(item);
    if (item)
    {
        emit treePropertyBrowserPtr_->expanded(idx);
    }
}

void TreePropertyBrowserPrivate::slotCurrentBrowserItemChanged(BrowserItem* item)
{
    if (!browserChangedBlocked_ && item != currentItem())
    {
        setCurrentItem(item, true);
    }
}

void TreePropertyBrowserPrivate::slotCurrentTreeItemChanged(QTreeWidgetItem* newItem, QTreeWidgetItem*)
{
    auto browserItem = newItem ? itemToIndex_.value(newItem) : nullptr;
    browserChangedBlocked_ = true;
    treePropertyBrowserPtr_->setCurrentItem(browserItem);
    browserChangedBlocked_ = false;
}

QTreeWidgetItem* TreePropertyBrowserPrivate::editedItem() const
{
    return delegate_->editedItem();
}

void TreePropertyBrowserPrivate::editItem(BrowserItem* browserItem)
{
    if (auto treeItem = indexToItem_.value(browserItem, 0))
    {
        treeWidget_->setCurrentItem (treeItem, 1);
        treeWidget_->editItem(treeItem, 1);
    }
}
