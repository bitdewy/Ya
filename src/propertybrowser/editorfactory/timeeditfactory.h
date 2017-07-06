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
#include "propertymanager/timepropertymanager.h"

class TimeEditFactoryPrivate;
class TimeEditFactory : public AbstractEditorFactory<TimePropertyManager>
{
    Q_OBJECT
public:
    explicit TimeEditFactory(QObject* parent = nullptr);
    ~TimeEditFactory();
protected:
    void connectPropertyManager(TimePropertyManager* manager);
    QWidget* createEditor(TimePropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(TimePropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, const QTime& value);
    QScopedPointer<TimeEditFactoryPrivate> impl_;
    friend class TimeEditFactoryPrivate;
};
