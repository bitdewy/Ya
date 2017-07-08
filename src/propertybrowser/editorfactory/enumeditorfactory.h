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
#include "propertymanager/enumpropertymanager.h"

class EnumEditorFactoryPrivate;
class EnumEditorFactory : public AbstractEditorFactory<EnumPropertyManager>
{
    Q_OBJECT
public:
    explicit EnumEditorFactory(QObject* parent = nullptr);
    ~EnumEditorFactory();
protected:
    void connectPropertyManager(EnumPropertyManager* manager);
    QWidget* createEditor(EnumPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(EnumPropertyManager* manager);
private:
    void slotPropertyChanged(Property *property, int value);
    void slotEnumNamesChanged(Property *property, const QStringList & enumNames);
    QScopedPointer<EnumEditorFactoryPrivate> impl_;
    friend class EnumEditorFactoryPrivate;
};
