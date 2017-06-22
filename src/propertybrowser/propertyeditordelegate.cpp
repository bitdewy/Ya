/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "propertyeditordelegate.h"
#include "propertyeditorview.h"
#include "private/treepropertybrowser_p.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QTreeWidgetItem>
#include <QtGui/QFocusEvent>
#include <QtGui/QPainter>

PropertyEditorDelegate::PropertyEditorDelegate(QObject* parent) :
    QItemDelegate(parent),
    editorPrivate_(0),
    editedItem_(0),
    editedWidget_(0)
{

}

int PropertyEditorDelegate::indentation(const QModelIndex& index) const
{
    if (!editorPrivate_)
    {
        return 0;
    }
    auto item = editorPrivate_->indexToItem(index);
    auto indent = 0;
    while (item->parent())
    {
        item = item->parent();
        ++indent;
    }
    if (editorPrivate_->treeWidget()->rootIsDecorated())
    {
        ++indent;
    }
    return indent * editorPrivate_->treeWidget()->indentation();
}

void PropertyEditorDelegate::slotEditorDestroyed(QObject* object)
{
    if (auto w = qobject_cast<QWidget*>(object))
    {
        const auto it = editorToProperty_.find(w);
        if (it != editorToProperty_.end())
        {
            propertyToEditor_.remove(it.value());
            editorToProperty_.erase(it);
        }
        if (editedWidget_ == w)
        {
            editedWidget_ = 0;
            editedItem_ = 0;
        }
    }
}

void PropertyEditorDelegate::closeEditor(Property* property)
{
    if (auto w = propertyToEditor_.value(property, nullptr))
    {
        w->deleteLater();
    }
}

QTreeWidgetItem *PropertyEditorDelegate::editedItem() const
{
    return editedItem_;
}

void PropertyEditorDelegate::setEditorPrivate(TreePropertyBrowserPrivate* editorPrivate)
{
    editorPrivate_ = editorPrivate;
}

QWidget* PropertyEditorDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem&, const QModelIndex& index) const
{
    if (index.column() == 1 && editorPrivate_)
    {
        auto property = editorPrivate_->indexToProperty(index);
        auto item = editorPrivate_->indexToItem(index);
        if (property && item && (item->flags() & Qt::ItemIsEnabled))
        {
            auto editor = editorPrivate_->createEditor(property, parent);
            if (editor)
            {
                editor->setAutoFillBackground(true);
                editor->installEventFilter(const_cast<PropertyEditorDelegate*>(this));
                connect(editor, &QWidget::destroyed,
                    this, &PropertyEditorDelegate::slotEditorDestroyed);
                propertyToEditor_[property] = editor;
                editorToProperty_[editor] = property;
                editedItem_ = item;
                editedWidget_ = editor;
            }
            return editor;
        }
    }
    return nullptr;
}

void PropertyEditorDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect.adjusted(0, 0, 0, -1));
}

void PropertyEditorDelegate::paint(QPainter* painter,
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto hasValue = true;
    if (editorPrivate_)
    {
        auto property = editorPrivate_->indexToProperty(index);
        if (property)
        {
            hasValue = property->hasValue();
        }
    }
    auto opt = option;
    if ((editorPrivate_ && index.column() == 0) || !hasValue)
    {
        auto property = editorPrivate_->indexToProperty(index);
        if (property && property->isModified())
        {
            opt.font.setBold(true);
            opt.fontMetrics = QFontMetrics(opt.font);
        }
    }
    QColor c;
    if (!hasValue && editorPrivate_->markPropertiesWithoutValue())
    {
        c = opt.palette.color(QPalette::Dark);
        opt.palette.setColor(QPalette::Text, opt.palette.color(QPalette::BrightText));
    }
    else
    {
        c = editorPrivate_->calculatedBackgroundColor(editorPrivate_->indexToBrowserItem(index));
        if (c.isValid() && (opt.features & QStyleOptionViewItem::Alternate))
        {
            c = c.lighter(112);
        }
    }
    if (c.isValid())
    {
        painter->fillRect(option.rect, c);
    }
    opt.state &= ~QStyle::State_HasFocus;
    QItemDelegate::paint(painter, opt, index);

    opt.palette.setCurrentColorGroup(QPalette::Active);
    auto color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &opt));
    painter->save();
    painter->setPen(QPen(color));
    if (!editorPrivate_ || (!editorPrivate_->lastColumn(index.column()) && hasValue))
    {
        auto right = (option.direction == Qt::LeftToRight) ? option.rect.right() : option.rect.left();
        painter->drawLine(right, option.rect.y(), right, option.rect.bottom());
    }
    painter->restore();
}

QSize PropertyEditorDelegate::sizeHint(const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    return QItemDelegate::sizeHint(option, index) + QSize(3, 4);
}

void PropertyEditorDelegate::setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const
{

}

void PropertyEditorDelegate::setEditorData(QWidget*, const QModelIndex&) const
{

}

bool PropertyEditorDelegate::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::FocusOut)
    {
        auto fe = static_cast<QFocusEvent*>(event);
        if (fe->reason() == Qt::ActiveWindowFocusReason)
        {
            return false;
        }
    }
    return QItemDelegate::eventFilter(object, event);
}
