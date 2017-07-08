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

class DoublePropertyManagerPrivate;
class DoublePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit DoublePropertyManager(QObject* parent = nullptr);
    ~DoublePropertyManager();

    double value(const Property* property) const;
    double minimum(const Property* property) const;
    double maximum(const Property* property) const;
    double singleStep(const Property* property) const;
    int decimals(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, double val);
    void setMinimum(Property* property, double minVal);
    void setMaximum(Property* property, double maxVal);
    void setRange(Property* property, double minVal, double maxVal);
    void setSingleStep(Property* property, double step);
    void setDecimals(Property* property, int prec);
Q_SIGNALS:
    void valueChanged(Property* property, double val);
    void rangeChanged(Property* property, double minVal, double maxVal);
    void singleStepChanged(Property* property, double step);
    void decimalsChanged(Property* property, int prec);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<DoublePropertyManagerPrivate> impl_;
    friend class DoublePropertyManagerPrivate;
};
