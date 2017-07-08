/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "timeeditfactory_p.h"
#include "editorfactory/timeeditfactory.h"

void TimeEditFactoryPrivate::slotPropertyChanged(Property* property, const QTime& value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setTime(value);
        editor->blockSignals(false);
    }
}

void TimeEditFactoryPrivate::slotSetValue(const QTime& value)
{
    auto&& object = timeEditFactoryPtr_->sender();
    const  auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            TimePropertyManager *manager = timeEditFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}

