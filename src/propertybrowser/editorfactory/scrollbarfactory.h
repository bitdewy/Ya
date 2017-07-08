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

class ScrollBarFactoryPrivate;
class ScrollBarFactory : public AbstractEditorFactory<IntPropertyManager>
{
    Q_OBJECT
public:
    explicit ScrollBarFactory(QObject* parent = nullptr);
    ~ScrollBarFactory();
protected:
    void connectPropertyManager(IntPropertyManager* manager);
    QWidget* createEditor(IntPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(IntPropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, int value);
    void slotRangeChanged(Property* property, int min, int max);
    void slotSingleStepChanged(Property* property, int step);

    QScopedPointer<ScrollBarFactoryPrivate> impl_;
    friend class ScrollBarFactoryPrivate;
};
