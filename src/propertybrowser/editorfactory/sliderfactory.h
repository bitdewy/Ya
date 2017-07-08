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

class SliderFactoryPrivate;
class SliderFactory : public AbstractEditorFactory<IntPropertyManager>
{
    Q_OBJECT
public:
    explicit SliderFactory(QObject* parent = nullptr);
    ~SliderFactory();

protected:
    void connectPropertyManager(IntPropertyManager* manager);
    QWidget* createEditor(IntPropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(IntPropertyManager* manager);

private:
    void slotPropertyChanged(Property* property, int value);
    void slotRangeChanged(Property* property, int min, int max);
    void slotSingleStepChanged(Property* property, int step);

    QScopedPointer<SliderFactoryPrivate> impl_;
    friend class SliderFactoryPrivate;
};
