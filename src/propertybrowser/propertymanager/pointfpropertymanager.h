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

class DoublePropertyManager;
class PointFPropertyManagerPrivate;
class PointFPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit PointFPropertyManager(QObject* parent = nullptr);
    ~PointFPropertyManager();

    DoublePropertyManager* subDoublePropertyManager() const;

    QPointF value(const Property* property) const;
    int decimals(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QPointF& val);
    void setDecimals(Property* property, int prec);
Q_SIGNALS:
    void valueChanged(Property* property, const QPointF& val);
    void decimalsChanged(Property* property, int prec);
protected:
    QString valueText(const Property *property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<PointFPropertyManagerPrivate> impl_;
    friend class PointFPropertyManagerPrivate;
};
