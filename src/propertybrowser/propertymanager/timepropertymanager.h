/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "abstractpropertymanager.h"

class TimePropertyManagerPrivate;
class TimePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    TimePropertyManager(QObject* parent = nullptr);
    ~TimePropertyManager();

    QTime value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QTime& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QTime& val);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<TimePropertyManagerPrivate> impl_;
    friend class TimePropertyManager;
};
