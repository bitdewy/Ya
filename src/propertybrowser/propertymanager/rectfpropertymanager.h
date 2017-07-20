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
class RectFPropertyManagerPrivate;
class RectFPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit RectFPropertyManager(QObject* parent = nullptr);
    ~RectFPropertyManager();

    DoublePropertyManager* subDoublePropertyManager() const;

    QRectF value(const Property* property) const;
    QRectF constraint(const Property* property) const;
    int decimals(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QRectF& val);
    void setConstraint(Property* property, const QRectF& constraint);
    void setDecimals(Property* property, int prec);
Q_SIGNALS:
    void valueChanged(Property* property, const QRectF& val);
    void constraintChanged(Property* property, const QRectF& constraint);
    void decimalsChanged(Property* property, int prec);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<RectFPropertyManagerPrivate> impl_;
    friend class RectFPropertyManagerPrivate;
};
