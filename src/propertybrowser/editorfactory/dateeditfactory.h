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
#include "propertymanager/datepropertymanager.h"

class DateEditFactoryPrivate;
class DateEditFactory : public AbstractEditorFactory<DatePropertyManager>
{
    Q_OBJECT
public:
    explicit DateEditFactory(QObject* parent = nullptr);
    ~DateEditFactory();
protected:
    void connectPropertyManager(DatePropertyManager* manager);
    QWidget* createEditor(DatePropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(DatePropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, const QDate& value);
    void slotRangeChanged(Property* property, const QDate& min, const QDate& max);
    QScopedPointer<DateEditFactoryPrivate> impl_;
    friend class DateEditFactoryPrivate;
};
