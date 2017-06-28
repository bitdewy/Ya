/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "fonteditorfactory_p.h"
#include "editorfactory/fonteditorfactory.h"

void FontEditorFactoryPrivate::slotPropertyChanged(Property* property,
    const QFont& value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    for (auto&& e : it.value())
    {
        e->setValue(value);
    }
}

void FontEditorFactoryPrivate::slotSetValue(const QFont& value)
{
    auto object = fontEditorFactoryPtr_->sender();
    const auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = fontEditorFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
