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
#include "propertymanager/colorpropertymanager.h"

class ColorEditorFactoryPrivate;
class ColorEditorFactory : public AbstractEditorFactory<ColorPropertyManager>
{
    Q_OBJECT
public:
    explicit ColorEditorFactory(QObject* parent = nullptr);
    ~ColorEditorFactory();
protected:
    void connectPropertyManager(ColorPropertyManager* manager);
    QWidget* createEditor(ColorPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(ColorPropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, const QColor& value);
    QScopedPointer<ColorEditorFactoryPrivate> impl_;
    friend class ColorEditorFactoryPrivate;
};
