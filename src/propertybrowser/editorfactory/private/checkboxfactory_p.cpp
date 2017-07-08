/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "checkboxfactory_p.h"
#include "editorfactory/checkboxfactory.h"

void CheckBoxFactoryPrivate::slotPropertyChanged(Property* property, bool value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.cend())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockCheckBoxSignals(true);
        editor->setChecked(value);
        editor->blockCheckBoxSignals(false);
    }
}

void CheckBoxFactoryPrivate::slotSetValue(bool value)
{
    auto object = checkBoxFactoryPtr_->sender();

    const auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend;  ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = checkBoxFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
