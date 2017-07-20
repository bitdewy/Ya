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
#include "propertymanager/variantpropertymanager.h"

class VariantEditorFactory : public AbstractEditorFactory<VariantPropertyManager>
{
    Q_OBJECT
public:
    explicit VariantEditorFactory(QObject* parent = nullptr);
    ~VariantEditorFactory();

protected:
    void connectPropertyManager(VariantPropertyManager* manager);
    QWidget* createEditor(VariantPropertyManager* manager, Property* property,
        QWidget* parent);
    void disconnectPropertyManager(VariantPropertyManager* manager);
private:
    QScopedPointer<class VariantEditorFactoryPrivate> impl_;
    friend class VariantEditorFactoryPrivate;
};
