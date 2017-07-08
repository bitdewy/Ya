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
#include "propertymanager/boolpropertymanager.h"

class CheckBoxFactoryPrivate;
class CheckBoxFactory : public AbstractEditorFactory<BoolPropertyManager>
{
    Q_OBJECT
public:
    explicit CheckBoxFactory(QObject* parent = nullptr);
    ~CheckBoxFactory();
protected:
    void connectPropertyManager(BoolPropertyManager* manager);
    QWidget* createEditor(BoolPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(BoolPropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, bool value);
    QScopedPointer<CheckBoxFactoryPrivate> impl_;
    friend class CheckBoxFactoryPrivate;
};
