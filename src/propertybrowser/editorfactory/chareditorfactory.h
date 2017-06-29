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
#include "propertymanager/charpropertymanager.h"

class CharEditorFactoryPrivate;
class CharEditorFactory : public AbstractEditorFactory<CharPropertyManager>
{
    Q_OBJECT
public:
    explicit CharEditorFactory(QObject* parent = nullptr);
    ~CharEditorFactory();
protected:
    void connectPropertyManager(CharPropertyManager* manager);
    QWidget* createEditor(CharPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(CharPropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, const QChar& value);
    QScopedPointer<CharEditorFactoryPrivate> impl_;
    friend class CharEditorFactoryPrivate;
};
