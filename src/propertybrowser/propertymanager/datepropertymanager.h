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

class DatePropertyManagerPrivate;
class DatePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit DatePropertyManager(QObject* parent = nullptr);
    ~DatePropertyManager();

    QDate value(const Property* property) const;
    QDate minimum(const Property* property) const;
    QDate maximum(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QDate& val);
    void setMinimum(Property* property, const QDate& minVal);
    void setMaximum(Property* property, const QDate& maxVal);
    void setRange(Property* property, const QDate& minVal, const QDate& maxVal);
Q_SIGNALS:
    void valueChanged(Property* property, const QDate& val);
    void rangeChanged(Property* property, const QDate& minVal, const QDate& maxVal);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<DatePropertyManagerPrivate> impl_;
    friend class DatePropertyManager;
};
