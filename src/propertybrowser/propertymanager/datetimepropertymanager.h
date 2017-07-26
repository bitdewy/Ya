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

class DateTimePropertyManagerPrivate;
class DateTimePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit DateTimePropertyManager(QObject* parent = nullptr);
    ~DateTimePropertyManager();

    QDateTime value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QDateTime& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QDateTime& val);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<DateTimePropertyManagerPrivate> impl_;
    friend class DateTimePropertyManagerPrivate;
};
