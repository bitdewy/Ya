/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "datetimeeditfactory_p.h"
#include "editorfactory/datetimeeditfactory.h"

void DateTimeEditFactoryPrivate::slotPropertyChanged(Property* property,
    const QDateTime& value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setDateTime(value);
        editor->blockSignals(false);
    }
}

void DateTimeEditFactoryPrivate::slotSetValue(const QDateTime& value)
{
    auto object = dateTimeEditPropertyPtr_->sender();
    const  auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend;  ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = dateTimeEditPropertyPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
