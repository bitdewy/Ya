/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QMap>
#include "cursordatabase.h"

class QCursor;
class QObject;
class QWidget;
class Property;
class EnumEditorFactory;
class EnumPropertyManager;
class CursorEditorFactory;
class CursorEditorFactoryPrivate
{
    CursorEditorFactory* cursorEditorFactoryPtr_;
    friend class CursorEditorFactory;
public:
    CursorEditorFactoryPrivate();
    CursorEditorFactoryPrivate(const CursorEditorFactoryPrivate&) = delete;
    CursorEditorFactoryPrivate& operator=(const CursorEditorFactoryPrivate&) = delete;

    void slotPropertyChanged(Property* property, const QCursor& cursor);
    void slotEnumChanged(Property* property, int value);
    void slotEditorDestroyed(QObject* object);

    EnumEditorFactory* enumEditorFactory_;
    EnumPropertyManager* enumPropertyManager_;

    QMap<Property*, Property*> propertyToEnum_;
    QMap<Property*, Property*> enumToProperty_;
    QMap<Property*, QList<QWidget*>> enumToEditors_;
    QMap<QWidget*, Property*> editorToEnum_;
    bool updatingEnum_;
};

Q_GLOBAL_STATIC(CursorDatabase, cursorDatabase)
