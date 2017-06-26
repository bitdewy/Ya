/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "groupboxpropertybrowser_p.h"
#include "groupboxpropertybrowser.h"
#include "browseritem.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>

void GroupBoxPropertyBrowserPrivate::init(QWidget* parent)
{
    mainLayout_ = new QGridLayout();
    parent->setLayout(mainLayout_);
    auto item = new QSpacerItem(0, 0,
        QSizePolicy::Fixed, QSizePolicy::Expanding);
    mainLayout_->addItem(item, 0, 0);
}

void GroupBoxPropertyBrowserPrivate::slotEditorDestroyed()
{
    auto editor = qobject_cast<QWidget*>(groupBoxPropertyBrowserPtr_->sender());
    if (!editor)
    {
        return;
    }
    if (!widgetToItem_.contains(editor))
    {
        return;
    }
    widgetToItem_[editor]->widget = nullptr;
    widgetToItem_.remove(editor);
}

void GroupBoxPropertyBrowserPrivate::slotUpdate()
{
    for (auto&& item : qAsConst(recreateQueue_))
    {
        auto par = item->parent;
        QWidget* w = nullptr;
        QGridLayout* l = nullptr;
        int oldRow = -1;
        if (!par)
        {
            w = groupBoxPropertyBrowserPtr_;
            l = mainLayout_;
            oldRow = children_.indexOf(item);
        }
        else
        {
            w = par->groupBox;
            l = par->layout;
            oldRow = par->children.indexOf(item);
            if (hasHeader(par))
            {
                oldRow += 2;
            }
        }

        if (item->widget)
        {
            item->widget->setParent(w);
        }
        else if (item->widgetLabel)
        {
            item->widgetLabel->setParent(w);
        }
        else
        {
            item->widgetLabel = new QLabel(w);
        }
        int span = 1;
        if (item->widget)
        {
            l->addWidget(item->widget, oldRow, 1, 1, 1);
        }
        else if (item->widgetLabel)
        {
            l->addWidget(item->widgetLabel, oldRow, 1, 1, 1);
        }
        else
        {
            span = 2;
        }
        item->label = new QLabel(w);
        item->label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        l->addWidget(item->label, oldRow, 0, 1, span);

        updateItem(item);
    }
    recreateQueue_.clear();
}

void GroupBoxPropertyBrowserPrivate::updateLater()
{
    QTimer::singleShot(0, groupBoxPropertyBrowserPtr_, SLOT(slotUpdate()));
}

void GroupBoxPropertyBrowserPrivate::propertyInserted(BrowserItem* index, BrowserItem* afterIndex)
{
    auto afterItem = indexToItem_.value(afterIndex);
    auto parentItem = indexToItem_.value(index->parent());

    auto newItem = new WidgetItem();
    newItem->parent = parentItem;

    QGridLayout* layout = nullptr;
    QWidget* parentWidget = nullptr;
    int row = -1;
    if (!afterItem)
    {
        row = 0;
        if (parentItem)
        {
            parentItem->children.insert(0, newItem);
        }
        else
        {
            children_.insert(0, newItem);
        }
    }
    else
    {
        if (parentItem)
        {
            row = parentItem->children.indexOf(afterItem) + 1;
            parentItem->children.insert(row, newItem);
        }
        else
        {
            row = children_.indexOf(afterItem) + 1;
            children_.insert(row, newItem);
        }
    }
    if (parentItem && hasHeader(parentItem))
    {
        row += 2;
    }
    if (!parentItem)
    {
        layout = mainLayout_;
        parentWidget = groupBoxPropertyBrowserPtr_;;
    }
    else
    {
        if (!parentItem->groupBox)
        {
            recreateQueue_.removeAll(parentItem);
            auto par = parentItem->parent;
            QWidget* w = nullptr;
            QGridLayout* l = nullptr;
            auto oldRow = -1;
            if (!par)
            {
                w = groupBoxPropertyBrowserPtr_;
                l = mainLayout_;
                oldRow = children_.indexOf(parentItem);
            }
            else
            {
                w = par->groupBox;
                l = par->layout;
                oldRow = par->children.indexOf(parentItem);
                if (hasHeader(par))
                {
                    oldRow += 2;
                }
            }
            parentItem->groupBox = new QGroupBox(w);
            parentItem->layout = new QGridLayout();
            parentItem->groupBox->setLayout(parentItem->layout);
            if (parentItem->label)
            {
                l->removeWidget(parentItem->label);
                delete parentItem->label;
                parentItem->label = 0;
            }
            if (parentItem->widget)
            {
                l->removeWidget(parentItem->widget);
                parentItem->widget->setParent(parentItem->groupBox);
                parentItem->layout->addWidget(parentItem->widget, 0, 0, 1, 2);
                parentItem->line = new QFrame(parentItem->groupBox);
            }
            else if (parentItem->widgetLabel)
            {
                l->removeWidget(parentItem->widgetLabel);
                delete parentItem->widgetLabel;
                parentItem->widgetLabel = 0;
            }
            if (parentItem->line)
            {
                parentItem->line->setFrameShape(QFrame::HLine);
                parentItem->line->setFrameShadow(QFrame::Sunken);
                parentItem->layout->addWidget(parentItem->line, 1, 0, 1, 2);
            }
            l->addWidget(parentItem->groupBox, oldRow, 0, 1, 2);
            updateItem(parentItem);
        }
        layout = parentItem->layout;
        parentWidget = parentItem->groupBox;
    }

    newItem->label = new QLabel(parentWidget);
    newItem->label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    newItem->widget = createEditor(index->property(), parentWidget);
    if (!newItem->widget)
    {
        newItem->widgetLabel = new QLabel(parentWidget);
    }
    else
    {
        QObject::connect(newItem->widget, &QWidget::destroyed,
            groupBoxPropertyBrowserPtr_, [this]() { slotEditorDestroyed(); });
        widgetToItem_[newItem->widget] = newItem;
    }

    insertRow(layout, row);
    auto span = 1;
    if (newItem->widget)
    {
        layout->addWidget(newItem->widget, row, 1);
    }
    else if (newItem->widgetLabel)
    {
        layout->addWidget(newItem->widgetLabel, row, 1);
    }
    else
    {
        span = 2;
    }
    layout->addWidget(newItem->label, row, 0, 1, span);

    itemToIndex_[newItem] = index;
    indexToItem_[index] = newItem;

    updateItem(newItem);
}

void GroupBoxPropertyBrowserPrivate::propertyRemoved(BrowserItem* index)
{
    auto item = indexToItem_.value(index);

    indexToItem_.remove(index);
    itemToIndex_.remove(item);

    auto parentItem = item->parent;

    int row = -1;

    if (parentItem)
    {
        row = parentItem->children.indexOf(item);
        parentItem->children.removeAt(row);
        if (hasHeader(parentItem))
        {
            row += 2;
        }
    }
    else
    {
        row = children_.indexOf(item);
        children_.removeAt(row);
    }

    if (item->widget)
    {
        delete item->widget;
    }
    if (item->label)
    {
        delete item->label;
    }
    if (item->widgetLabel)
    {
        delete item->widgetLabel;
    }
    if (item->groupBox)
    {
        delete item->groupBox;
    }

    if (!parentItem)
    {
        removeRow(mainLayout_, row);
    }
    else if (parentItem->children.count() != 0)
    {
        removeRow(parentItem->layout, row);
    }
    else
    {
        auto par = parentItem->parent;
        QGridLayout* l = nullptr;
        auto oldRow = -1;
        if (!par)
        {
            l = mainLayout_;
            oldRow = children_.indexOf(parentItem);
        }
        else
        {
            l = par->layout;
            oldRow = par->children.indexOf(parentItem);
            if (hasHeader(par))
            {
                oldRow += 2;
            }
        }

        if (parentItem->widget)
        {
            parentItem->widget->hide();
            parentItem->widget->setParent(0);
        }
        else if (parentItem->widgetLabel)
        {
            parentItem->widgetLabel->hide();
            parentItem->widgetLabel->setParent(0);
        }
        else
        {
        }
        l->removeWidget(parentItem->groupBox);
        delete parentItem->groupBox;
        parentItem->groupBox = 0;
        parentItem->line = 0;
        parentItem->layout = 0;
        if (!recreateQueue_.contains(parentItem))
        {
            recreateQueue_.append(parentItem);
        }
        updateLater();
    }
    recreateQueue_.removeAll(item);

    delete item;
}

void GroupBoxPropertyBrowserPrivate::insertRow(QGridLayout* layout, int row) const
{
    QMap<QLayoutItem*, QRect> itemToPos;
    auto idx = 0;
    while (idx < layout->count())
    {
        int r, c, rs, cs;
        layout->getItemPosition(idx, &r, &c, &rs, &cs);
        if (r >= row)
        {
            itemToPos[layout->takeAt(idx)] = QRect(r + 1, c, rs, cs);
        }
        else
        {
            idx++;
        }
    }

    const auto icend = itemToPos.constEnd();
    for (auto it = itemToPos.constBegin(); it != icend; ++it)
    {
        const auto r = it.value();
        layout->addItem(it.key(), r.x(), r.y(), r.width(), r.height());
    }
}

void GroupBoxPropertyBrowserPrivate::removeRow(QGridLayout* layout, int row) const
{
    QMap<QLayoutItem*, QRect> itemToPos;
    int idx = 0;
    while (idx < layout->count())
    {
        int r, c, rs, cs;
        layout->getItemPosition(idx, &r, &c, &rs, &cs);
        if (r > row)
        {
            itemToPos[layout->takeAt(idx)] = QRect(r - 1, c, rs, cs);
        } else
        {
            idx++;
        }
    }

    const auto icend = itemToPos.constEnd();
    for (auto it = itemToPos.constBegin(); it != icend; ++it)
    {
        const auto r = it.value();
        layout->addItem(it.key(), r.x(), r.y(), r.width(), r.height());
    }
}

bool GroupBoxPropertyBrowserPrivate::hasHeader(WidgetItem* item) const
{
    return !!item->widget;
}

void GroupBoxPropertyBrowserPrivate::propertyChanged(BrowserItem* index)
{
    auto item = indexToItem_.value(index);
    updateItem(item);
}

QWidget* GroupBoxPropertyBrowserPrivate::createEditor(Property* property, QWidget* parent) const
{
    return groupBoxPropertyBrowserPtr_->createEditor(property, parent);
}

void GroupBoxPropertyBrowserPrivate::updateItem(WidgetItem* item)
{
    auto property = itemToIndex_[item]->property();
    if (item->groupBox)
    {
        auto font = item->groupBox->font();
        font.setUnderline(property->isModified());
        item->groupBox->setFont(font);
        item->groupBox->setTitle(property->propertyName());
        item->groupBox->setToolTip(property->descriptionToolTip());
        item->groupBox->setStatusTip(property->statusTip());
        item->groupBox->setWhatsThis(property->whatsThis());
        item->groupBox->setEnabled(property->isEnabled());
    }
    if (item->label)
    {
        auto font = item->label->font();
        font.setUnderline(property->isModified());
        item->label->setFont(font);
        item->label->setText(property->propertyName());
        item->label->setToolTip(property->descriptionToolTip());
        item->label->setStatusTip(property->statusTip());
        item->label->setWhatsThis(property->whatsThis());
        item->label->setEnabled(property->isEnabled());
    }
    if (item->widgetLabel)
    {
        auto font = item->widgetLabel->font();
        font.setUnderline(false);
        item->widgetLabel->setFont(font);
        item->widgetLabel->setText(property->valueText());
        item->widgetLabel->setEnabled(property->isEnabled());
    }
    if (item->widget)
    {
        auto font = item->widget->font();
        font.setUnderline(false);
        item->widget->setFont(font);
        item->widget->setEnabled(property->isEnabled());
        const auto toolTip = property->toolTip();
        item->widget->setToolTip(toolTip.isEmpty() ? property->valueText() : toolTip);
    }
}
