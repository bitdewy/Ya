/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtWidgets/QTreeWidget>

class TreePropertyBrowserPrivate;
class PropertyEditorView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PropertyEditorView(QWidget* parent = nullptr);

    void setEditorPrivate(TreePropertyBrowserPrivate* editorPrivate);

    QTreeWidgetItem* indexToItem(const QModelIndex& index) const;

protected:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    TreePropertyBrowserPrivate* editorPrivate_;
};
