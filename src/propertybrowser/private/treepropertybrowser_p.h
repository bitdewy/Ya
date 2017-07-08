/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "treepropertybrowser.h"
#include <QtCore/QMap>
#include <QtGui/QIcon>

class QWidget;
class QModelIndex;
class QTreeWidgetItem;
class Property;
class BrowserItem;
class TreePropertyBrowser;
class PropertyEditorView;
class TreePropertyBrowserPrivate
{
    TreePropertyBrowser* treePropertyBrowserPtr_;
    friend class TreePropertyBrowser;

public:
    TreePropertyBrowserPrivate();
    TreePropertyBrowserPrivate(const TreePropertyBrowserPrivate&) = delete;
    TreePropertyBrowserPrivate& operator=(const TreePropertyBrowserPrivate&) = delete;

    void init(QWidget* parent);

    void propertyInserted(BrowserItem* index, BrowserItem* afterIndex);
    void propertyRemoved(BrowserItem* index);
    void propertyChanged(BrowserItem* index);
    QWidget* createEditor(Property* property, QWidget* parent) const;

    Property* indexToProperty(const QModelIndex& index) const;
    QTreeWidgetItem* indexToItem(const QModelIndex& index) const;
    BrowserItem* indexToBrowserItem(const QModelIndex& index) const;
    bool lastColumn(int column) const;
    void disableItem(QTreeWidgetItem *item) const;
    void enableItem(QTreeWidgetItem *item) const;
    bool hasValue(QTreeWidgetItem *item) const;

    void slotCollapsed(const QModelIndex& index);
    void slotExpanded(const QModelIndex& index);

    QColor calculatedBackgroundColor(BrowserItem* item) const;

    PropertyEditorView* treeWidget() const;
    bool markPropertiesWithoutValue() const;

    BrowserItem* currentItem() const;
    void setCurrentItem(BrowserItem* browserItem, bool block);
    void editItem(BrowserItem* browserItem);

    void slotCurrentBrowserItemChanged(BrowserItem* item);
    void slotCurrentTreeItemChanged(QTreeWidgetItem* newItem, QTreeWidgetItem*);

    QTreeWidgetItem* editedItem() const;

private:
    void updateItem(QTreeWidgetItem* item);

    QMap<BrowserItem*, QTreeWidgetItem*> indexToItem_;
    QMap<QTreeWidgetItem*, BrowserItem*> itemToIndex_;

    QMap<BrowserItem*, QColor> indexToBackgroundColor_;

    PropertyEditorView* treeWidget_;

    bool headerVisible_;
    TreePropertyBrowser::ResizeMode resizeMode_;
    class PropertyEditorDelegate* delegate_;
    bool markPropertiesWithoutValue_;
    bool browserChangedBlocked_;
    QIcon expandIcon_;
};
