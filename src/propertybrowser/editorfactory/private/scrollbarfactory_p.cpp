/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "scrollbarfactory_p.h"
#include "editorfactory/scrollbarfactory.h"

void ScrollBarFactoryPrivate::slotPropertyChanged(Property* property, int value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.cend())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setValue(value);
        editor->blockSignals(false);
    }
}

void ScrollBarFactoryPrivate::slotRangeChanged(Property* property, int min, int max)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.cend())
    {
        return;
    }
    auto manager = scrollBarFactoryPtr_->propertyManager(property);
    if (!manager)
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setRange(min, max);
        editor->setValue(manager->value(property));
        editor->blockSignals(false);
    }
}

void ScrollBarFactoryPrivate::slotSingleStepChanged(Property* property, int step)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.cend())
    {
        return;
    }
    for (auto editor : it.value())
    {
        editor->blockSignals(true);
        editor->setSingleStep(step);
        editor->blockSignals(false);
    }
}

void ScrollBarFactoryPrivate::slotSetValue(int value)
{
    auto object = scrollBarFactoryPtr_->sender();
    const auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = scrollBarFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
