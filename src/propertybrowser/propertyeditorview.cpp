/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "propertyeditorview.h"
#include "private/treepropertybrowser_p.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>

PropertyEditorView::PropertyEditorView(QWidget* parent) :
    QTreeWidget(parent),
    editorPrivate_(nullptr)
{
    connect(header(), &QHeaderView::sectionDoubleClicked,
        this, &PropertyEditorView::resizeColumnToContents);
}

void PropertyEditorView::setEditorPrivate(TreePropertyBrowserPrivate* editorPrivate)
{
    editorPrivate_ = editorPrivate;
}

QTreeWidgetItem* PropertyEditorView::indexToItem(const QModelIndex& index) const
{
    return itemFromIndex(index);
}

void PropertyEditorView::drawRow(QPainter* painter,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto opt = option;
    bool hasValue = true;
    if (editorPrivate_)
    {
        auto property = editorPrivate_->indexToProperty(index);
        if (property)
        {
            hasValue = property->hasValue();
        }
    }
    if (!hasValue && editorPrivate_->markPropertiesWithoutValue())
    {
        const auto c = option.palette.color(QPalette::Dark);
        painter->fillRect(option.rect, c);
        opt.palette.setColor(QPalette::AlternateBase, c);
    }
    else
    {
        const auto c = editorPrivate_->calculatedBackgroundColor(editorPrivate_->indexToBrowserItem(index));
        if (c.isValid())
        {
            painter->fillRect(option.rect, c);
            opt.palette.setColor(QPalette::AlternateBase, c.lighter(112));
        }
    }
    QTreeWidget::drawRow(painter, opt, index);
    auto color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &opt));
    painter->save();
    painter->setPen(QPen(color));
    painter->drawLine(opt.rect.x(), opt.rect.bottom(), opt.rect.right(), opt.rect.bottom());
    painter->restore();
}

void PropertyEditorView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Space:
        if (!editorPrivate_->editedItem())
        {
            if (const QTreeWidgetItem *item = currentItem())
            {
                if (item->columnCount() >= 2 &&
                    ((item->flags() & (Qt::ItemIsEditable | Qt::ItemIsEnabled)) == (Qt::ItemIsEditable | Qt::ItemIsEnabled)))
                {
                    event->accept();
                    auto index = currentIndex();
                    if (index.column() == 0)
                    {
                        index = index.sibling(index.row(), 1);
                        setCurrentIndex(index);
                    }
                    edit(index);
                    return;
                }
            }
        }
        break;
    default:
        break;
    }
    QTreeWidget::keyPressEvent(event);
}

void PropertyEditorView::mousePressEvent(QMouseEvent* event)
{
    QTreeWidget::mousePressEvent(event);
    auto item = itemAt(event->pos());

    if (item)
    {
        if ((item != editorPrivate_->editedItem()) && (event->button() == Qt::LeftButton)
                && (header()->logicalIndexAt(event->pos().x()) == 1)
                && ((item->flags() & (Qt::ItemIsEditable | Qt::ItemIsEnabled)) == (Qt::ItemIsEditable | Qt::ItemIsEnabled)))
        {
            editItem(item, 1);
        }
        else if (!editorPrivate_->hasValue(item) && editorPrivate_->markPropertiesWithoutValue() && !rootIsDecorated())
        {
            if (event->pos().x() + header()->offset() < 20)
            {
                item->setExpanded(!item->isExpanded());
            }
        }
    }
}
