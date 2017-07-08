/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtWidgets/QItemDelegate>

class Property;
class QTreeWidgetItem;
class TreePropertyBrowserPrivate;
class PropertyEditorDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit PropertyEditorDelegate(QObject* parent = nullptr);

    void setEditorPrivate(TreePropertyBrowserPrivate* editorPrivate);

    QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option,
        const QModelIndex &index) const;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
        const QModelIndex& index) const;

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

    void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const;

    void setEditorData(QWidget*, const QModelIndex&) const;

    bool eventFilter(QObject* object, QEvent* event);
    void closeEditor(Property* property);

    QTreeWidgetItem* editedItem() const;

private slots:
    void slotEditorDestroyed(QObject* object);

private:
    int indentation(const QModelIndex&index) const;

    typedef QMap<QWidget*, Property*> EditorToPropertyMap;
    mutable EditorToPropertyMap editorToProperty_;

    typedef QMap<Property*, QWidget*> PropertyToEditorMap;
    mutable PropertyToEditorMap propertyToEditor_;
    TreePropertyBrowserPrivate* editorPrivate_;
    mutable QTreeWidgetItem* editedItem_;
    mutable QWidget* editedWidget_;
};
