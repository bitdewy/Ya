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
#include "propertymanager/doublepropertymanager.h"

class DoubleSpinBoxFactoryPrivate;

class DoubleSpinBoxFactory : public AbstractEditorFactory<DoublePropertyManager>
{
    Q_OBJECT
public:
    explicit DoubleSpinBoxFactory(QObject* parent = nullptr);
    ~DoubleSpinBoxFactory();
protected:
    void connectPropertyManager(DoublePropertyManager* manager);
    QWidget* createEditor(DoublePropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(DoublePropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, double value);
    void slotRangeChanged(Property* property, double min, double max);
    void slotSingleStepChanged(Property* property, double step);
    void slotDecimalsChanged(Property* property, int prec);
    QScopedPointer<DoubleSpinBoxFactoryPrivate> impl_;
    friend class DoubleSpinBoxFactoryPrivate;
};
