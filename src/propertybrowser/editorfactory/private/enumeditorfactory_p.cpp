/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "enumeditorfactory_p.h"
#include "editorfactory/enumeditorfactory.h"

void EnumEditorFactoryPrivate::slotPropertyChanged(Property* property, int value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setCurrentIndex(value);
        editor->blockSignals(false);
    }
}

void EnumEditorFactoryPrivate::slotEnumNamesChanged(Property* property,
    const QStringList& enumNames)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    auto manager = enumEditorFactoryPtr_->propertyManager(property);
    if (!manager)
    {
        return;
    }
    auto enumIcons = manager->enumIcons(property);

    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->clear();
        editor->addItems(enumNames);
        const auto nameCount = enumNames.count();
        for (auto i = 0; i < nameCount; i++)
        {
            editor->setItemIcon(i, enumIcons.value(i));
        }
        editor->setCurrentIndex(manager->value(property));
        editor->blockSignals(false);
    }
}

void EnumEditorFactoryPrivate::slotEnumIconsChanged(Property* property,
    const QMap<int, QIcon>& enumIcons)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    auto&& manager = enumEditorFactoryPtr_->propertyManager(property);
    if (!manager)
    {
        return;
    }
    const auto enumNames = manager->enumNames(property);
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        const auto nameCount = enumNames.count();
        for (auto i = 0; i < nameCount; i++)
        {
            editor->setItemIcon(i, enumIcons.value(i));
        }
        editor->setCurrentIndex(manager->value(property));
        editor->blockSignals(false);
    }
}

void EnumEditorFactoryPrivate::slotSetValue(int value)
{
    auto object = enumEditorFactoryPtr_->sender();
    const  auto ecend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = enumEditorFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
