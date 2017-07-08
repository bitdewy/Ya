/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "buttonpropertybrowser_p.h"
#include "property.h"
#include "browseritem.h"
#include "buttonpropertybrowser.h"
#include <QtCore/QTimer>
#include <QtWidgets/QGridlayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLabel>

QToolButton* ButtonPropertyBrowserPrivate::createButton(QWidget* parent) const
{
    auto button = new QToolButton(parent);
    button->setCheckable(true);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setArrowType(Qt::DownArrow);
    button->setIconSize(QSize(3, 16));
    return button;
}

int ButtonPropertyBrowserPrivate::gridRow(WidgetItem* item) const
{
    QList<WidgetItem*> siblings;
    if (item->parent)
    {
        siblings = item->parent->children;
    }
    else
    {
        siblings = children_;
    }
    auto row = 0;
    for (const auto& sibling : qAsConst(siblings))
    {
        if (sibling == item)
        {
            return row;
        }
        row += gridSpan(sibling);
    }
    return -1;
}

int ButtonPropertyBrowserPrivate::gridSpan(WidgetItem* item) const
{
    if (item->container && item->expanded)
    {
        return 2;
    }
    return 1;
}

ButtonPropertyBrowserPrivate::ButtonPropertyBrowserPrivate()
{

}

ButtonPropertyBrowserPrivate::~ButtonPropertyBrowserPrivate()
{

}

void ButtonPropertyBrowserPrivate::init(QWidget* parent)
{
    mainLayout_ = new QGridLayout();
    parent->setLayout(mainLayout_);
    auto item = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    mainLayout_->addItem(item, 0, 0);
}

void ButtonPropertyBrowserPrivate::slotEditorDestroyed()
{
    auto editor = qobject_cast<QWidget*>(buttonPropertyBrowserPtr_->sender());
    if (!editor)
    {
        return;
    }
    if (!widgetToItem_.contains(editor))
    {
        return;
    }
    widgetToItem_[editor]->widget = 0;
    widgetToItem_.remove(editor);
}

void ButtonPropertyBrowserPrivate::slotUpdate()
{
    for (const auto& item : qAsConst(recreateQueue_))
    {
        auto parent = item->parent;
        QWidget* w = nullptr;
        QGridLayout* l = nullptr;
        const int oldRow = gridRow(item);
        if (parent)
        {
            w = parent->container;
            l = parent->layout;
        }
        else
        {
            w = buttonPropertyBrowserPtr_;
            l = mainLayout_;
        }

        int span = 1;
        if (!item->widget && !item->widgetLabel)
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

void ButtonPropertyBrowserPrivate::setExpanded(WidgetItem* item, bool expanded)
{
    if (item->expanded == expanded)
    {
        return;
    }

    if (!item->container)
    {
        return;
    }

    item->expanded = expanded;
    const int row = gridRow(item);
    auto parent = item->parent;
    QGridLayout* l = nullptr;
    if (parent)
    {
        l = parent->layout;
    }
    else
    {
        l = mainLayout_;
    }

    if (expanded)
    {
        insertRow(l, row + 1);
        l->addWidget(item->container, row + 1, 0, 1, 2);
        item->container->show();
    }
    else
    {
        l->removeWidget(item->container);
        item->container->hide();
        removeRow(l, row + 1);
    }

    item->button->setChecked(expanded);
    item->button->setArrowType(expanded ? Qt::UpArrow : Qt::DownArrow);
}

void ButtonPropertyBrowserPrivate::slotToggled(bool checked)
{
    auto item = buttonToItem_.value(buttonPropertyBrowserPtr_->sender());
    if (!item)
    {
        return;
    }

    setExpanded(item, checked);

    if (checked)
    {
        emit buttonPropertyBrowserPtr_->expanded(itemToIndex_.value(item));
    }
    else
    {
        emit buttonPropertyBrowserPtr_->collapsed(itemToIndex_.value(item));
    }
}

void ButtonPropertyBrowserPrivate::updateLater()
{
    QTimer::singleShot(0, buttonPropertyBrowserPtr_, [this]() { slotUpdate(); });
}

void ButtonPropertyBrowserPrivate::propertyInserted(BrowserItem *index, BrowserItem *afterIndex)
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
        row = gridRow(afterItem) + gridSpan(afterItem);
        if (parentItem)
        {
            parentItem->children.insert(parentItem->children.indexOf(afterItem) + 1, newItem);
        }
        else
        {
            children_.insert(children_.indexOf(afterItem) + 1, newItem);
        }
    }

    if (!parentItem)
    {
        layout = mainLayout_;
        parentWidget = buttonPropertyBrowserPtr_;
    }
    else
    {
        if (!parentItem->container)
        {
            recreateQueue_.removeAll(parentItem);
            auto grandParent = parentItem->parent;
            QGridLayout* l = nullptr;
            const int oldRow = gridRow(parentItem);
            if (grandParent)
            {
                l = grandParent->layout;
            }
            else
            {
                l = mainLayout_;
            }
            auto container = new QFrame();
            container->setFrameShape(QFrame::Panel);
            container->setFrameShadow(QFrame::Raised);
            parentItem->container = container;
            parentItem->button = createButton();
            buttonToItem_[parentItem->button] = parentItem;
            buttonPropertyBrowserPtr_->connect(parentItem->button, &QToolButton::toggled,
                buttonPropertyBrowserPtr_, [this](bool toggled) { slotToggled(toggled); });
            parentItem->layout = new QGridLayout();
            container->setLayout(parentItem->layout);
            if (parentItem->label)
            {
                l->removeWidget(parentItem->label);
                delete parentItem->label;
                parentItem->label = 0;
            }
            int span = 1;
            if (!parentItem->widget && !parentItem->widgetLabel)
            {
                span = 2;
            }
            l->addWidget(parentItem->button, oldRow, 0, 1, span);
            updateItem(parentItem);
        }
        layout = parentItem->layout;
        parentWidget = parentItem->container;
    }

    newItem->label = new QLabel(parentWidget);
    newItem->label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    newItem->widget = createEditor(index->property(), parentWidget);
    if (newItem->widget)
    {
        QObject::connect(newItem->widget, &QWidget::destroyed,
            buttonPropertyBrowserPtr_, [this]() { slotEditorDestroyed(); });
        widgetToItem_[newItem->widget] = newItem;
    }
    else if (index->property()->hasValue())
    {
        newItem->widgetLabel = new QLabel(parentWidget);
        newItem->widgetLabel->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed));
    }

    insertRow(layout, row);
    int span = 1;
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
    layout->addWidget(newItem->label, row, 0, span, 1);

    itemToIndex_[newItem] = index;
    indexToItem_[index] = newItem;

    updateItem(newItem);
}

void ButtonPropertyBrowserPrivate::propertyRemoved(BrowserItem* index)
{
    auto item = indexToItem_.value(index);

    indexToItem_.remove(index);
    itemToIndex_.remove(item);

    auto parentItem = item->parent;

    const int row = gridRow(item);

    if (parentItem)
    {
        parentItem->children.removeAt(parentItem->children.indexOf(item));
    }
    else
    {
        children_.removeAt(children_.indexOf(item));
    }

    const int colSpan = gridSpan(item);

    buttonToItem_.remove(item->button);

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
    if (item->button)
    {
        delete item->button;
    }
    if (item->container)
    {
        delete item->container;
    }

    if (!parentItem)
    {
        removeRow(mainLayout_, row);
        if (colSpan > 1)
        {
            removeRow(mainLayout_, row);
        }
    }
    else if (parentItem->children.count() != 0)
    {
        removeRow(parentItem->layout, row);
        if (colSpan > 1)
        {
            removeRow(parentItem->layout, row);
        }
    }
    else
    {
        const auto grandParent = parentItem->parent;
        QGridLayout* l = nullptr;
        if (grandParent)
        {
            l = grandParent->layout;
        }
        else
        {
            l = mainLayout_;
        }

        const int parentRow = gridRow(parentItem);
        const int parentSpan = gridSpan(parentItem);

        l->removeWidget(parentItem->button);
        l->removeWidget(parentItem->container);
        delete parentItem->button;
        delete parentItem->container;
        parentItem->button = 0;
        parentItem->container = 0;
        parentItem->layout = 0;
        if (!recreateQueue_.contains(parentItem))
        {
            recreateQueue_.append(parentItem);
        }
        if (parentSpan > 1)
        {
            removeRow(l, parentRow + 1);
        }

        updateLater();
    }
    recreateQueue_.removeAll(item);

    delete item;
}

void ButtonPropertyBrowserPrivate::insertRow(QGridLayout* layout, int row) const
{
    QMap<QLayoutItem *, QRect> itemToPos;
    int idx = 0;
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

    for (auto it = itemToPos.constBegin(), icend = itemToPos.constEnd(); it != icend; ++it)
    {
        const auto r = it.value();
        layout->addItem(it.key(), r.x(), r.y(), r.width(), r.height());
    }
}

void ButtonPropertyBrowserPrivate::removeRow(QGridLayout* layout, int row) const
{
    QMap<QLayoutItem *, QRect> itemToPos;
    int idx = 0;
    while (idx < layout->count())
    {
        int r, c, rs, cs;
        layout->getItemPosition(idx, &r, &c, &rs, &cs);
        if (r > row)
        {
            itemToPos[layout->takeAt(idx)] = QRect(r - 1, c, rs, cs);
        }
        else
        {
            idx++;
        }
    }

    for (auto it = itemToPos.constBegin(), icend = itemToPos.constEnd(); it != icend; ++it)
    {
        const auto r = it.value();
        layout->addItem(it.key(), r.x(), r.y(), r.width(), r.height());
    }
}

void ButtonPropertyBrowserPrivate::propertyChanged(BrowserItem *index)
{
    auto item = indexToItem_.value(index);
    updateItem(item);
}

QWidget* ButtonPropertyBrowserPrivate::createEditor(Property* property, QWidget* parent) const
{
    return buttonPropertyBrowserPtr_->createEditor(property, parent);
}

void ButtonPropertyBrowserPrivate::updateItem(WidgetItem *item)
{
    auto property = itemToIndex_[item]->property();
    if (item->button)
    {
        auto font = item->button->font();
        font.setUnderline(property->isModified());
        item->button->setFont(font);
        item->button->setText(property->propertyName());
        item->button->setToolTip(property->descriptionToolTip());
        item->button->setStatusTip(property->statusTip());
        item->button->setWhatsThis(property->whatsThis());
        item->button->setEnabled(property->isEnabled());
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
        item->widgetLabel->setToolTip(property->valueText());
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
