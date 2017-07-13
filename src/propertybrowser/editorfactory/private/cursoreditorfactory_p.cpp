/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "cursoreditorfactory_p.h"
#include "editorfactory/cursoreditorfactory.h"
#include "propertymanager/enumpropertymanager.h"
#include <QtGui/QCursor>
#include <QtWidgets/QWidget>

CursorEditorFactoryPrivate::CursorEditorFactoryPrivate() :
    updatingEnum_(false)
{

}

void CursorEditorFactoryPrivate::slotPropertyChanged(Property* property, const QCursor& cursor)
{
    auto enumProp = propertyToEnum_.value(property);
    if (!enumProp)
    {
        return;
    }
    updatingEnum_ = true;
    enumPropertyManager_->setValue(enumProp, cursorDatabase()->cursorToValue(cursor));
    updatingEnum_ = false;
}

void CursorEditorFactoryPrivate::slotEnumChanged(Property* property, int value)
{
    if (updatingEnum_)
    {
        return;
    }
    auto prop = enumToProperty_.value(property);
    if (!prop)
    {
        return;
    }
    auto cursorManager = cursorEditorFactoryPtr_->propertyManager(prop);
    if (!cursorManager)
    {
        return;
    }
    cursorManager->setValue(prop, QCursor(cursorDatabase()->valueToCursor(value)));
}

void CursorEditorFactoryPrivate::slotEditorDestroyed(QObject* object)
{
    const auto ecend = editorToEnum_.constEnd();
    for (auto itEditor = editorToEnum_.constBegin(); itEditor != ecend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto editor = itEditor.key();
            auto enumProp = itEditor.value();
            editorToEnum_.remove(editor);
            enumToEditors_[enumProp].removeAll(editor);
            if (enumToEditors_[enumProp].isEmpty())
            {
                enumToEditors_.remove(enumProp);
                auto property = enumToProperty_.value(enumProp);
                enumToProperty_.remove(enumProp);
                propertyToEnum_.remove(property);
                delete enumProp;
            }
            return;
        }
    }
}
