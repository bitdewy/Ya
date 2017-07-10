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
#include "propertymanager/datetimepropertymanager.h"

class DateTimeEditFactoryPrivate;
class DateTimeEditFactory : public AbstractEditorFactory<DateTimePropertyManager>
{
    Q_OBJECT
public:
    explicit DateTimeEditFactory(QObject* parent = nullptr);
    ~DateTimeEditFactory();
protected:
    void connectPropertyManager(DateTimePropertyManager* manager);
    QWidget* createEditor(DateTimePropertyManager* manager, Property* property, QWidget* parent);
    void disconnectPropertyManager(DateTimePropertyManager* manager);
private:
    void slotPropertyChanged(Property* property, const QDateTime& value);
    QScopedPointer<DateTimeEditFactoryPrivate> impl_;
    friend class DateTimeEditFactoryPrivate;
};
