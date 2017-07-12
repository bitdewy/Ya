/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "coloreditorfactory_p.h"
#include "editorfactory/coloreditorfactory.h"

void ColorEditorFactoryPrivate::slotPropertyChanged(Property* property,
    const QColor& value)
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

void ColorEditorFactoryPrivate::slotSetValue(const QColor& value)
{
    auto object = colorEditorFactoryPtr_->sender();
    const auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = colorEditorFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
