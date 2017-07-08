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
#include "propertymanager/intpropertymanager.h"

class SpinBoxFactoryPrivate;
class SpinBoxFactory : public AbstractEditorFactory<IntPropertyManager>
{
    Q_OBJECT
public:
    explicit SpinBoxFactory(QObject* parent = nullptr);
    ~SpinBoxFactory();

protected:
    void connectPropertyManager(IntPropertyManager* manager);
    QWidget* createEditor(IntPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(IntPropertyManager* manager);

private:
    void slotPropertyChanged(Property* property, int value);
    void slotRangeChanged(Property* property, int min, int max);
    void slotSingleStepChanged(Property* property, int step);

    QScopedPointer<SpinBoxFactoryPrivate> impl_;
    friend class SpinBoxFactoryPrivate;
};
