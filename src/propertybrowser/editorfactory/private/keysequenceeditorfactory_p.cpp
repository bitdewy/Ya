/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "keysequenceeditorfactory_p.h"
#include "editorfactory/keysequenceeditorfactory.h"

void KeySequenceEditorFactoryPrivate::slotPropertyChanged(Property* property,
    const QKeySequence& value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setKeySequence(value);
        editor->blockSignals(false);
    }
}

void KeySequenceEditorFactoryPrivate::slotSetValue(const QKeySequence& value)
{
    auto object = keySequenceEditorFactoryPtr_->sender();
    const auto ecend = editorToProperty_.constEnd();
    for (auto itEditor =  editorToProperty_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = keySequenceEditorFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
