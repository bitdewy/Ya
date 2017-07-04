/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "dateeditfactory_p.h"
#include "editorfactory/dateeditfactory.h"

void DateEditFactoryPrivate::slotPropertyChanged(Property* property, const QDate& value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setDate(value);
        editor->blockSignals(false);
    }
}

void DateEditFactoryPrivate::slotRangeChanged(Property* property,
    const QDate& min, const QDate& max)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    auto manager = dateEditFactoryPtr_->propertyManager(property);
    if (!manager)
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setDateRange(min, max);
        editor->setDate(manager->value(property));
        editor->blockSignals(false);
    }
}

void DateEditFactoryPrivate::slotSetValue(const QDate& value)
{
    auto object = dateEditFactoryPtr_->sender();
    const auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = dateEditFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
