/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstracteditorfactory.h"
#include "propertymanager/cursorpropertymanager.h"

class CursorEditorFactoryPrivate;
class CursorEditorFactory : public AbstractEditorFactory<CursorPropertyManager>
{
    Q_OBJECT
public:
    explicit CursorEditorFactory(QObject* parent = nullptr);
    ~CursorEditorFactory();
protected:
    void connectPropertyManager(CursorPropertyManager* manager);
    QWidget* createEditor(CursorPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(CursorPropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, const QCursor& cursor);

    QScopedPointer<CursorEditorFactoryPrivate> impl_;
    friend class CursorEditorFactoryPrivate;
};
