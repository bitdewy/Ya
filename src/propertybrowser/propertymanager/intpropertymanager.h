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

class IntPropertyManagerPrivate;
class IntPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit IntPropertyManager(QObject* parent = nullptr);
    ~IntPropertyManager();

    int value(const Property* property) const;
    int minimum(const Property* property) const;
    int maximum(const Property* property) const;
    int singleStep(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, int val);
    void setMinimum(Property* property, int minVal);
    void setMaximum(Property* property, int maxVal);
    void setRange(Property* property, int minVal, int maxVal);
    void setSingleStep(Property* property, int step);
Q_SIGNALS:
    void valueChanged(Property* property, int val);
    void rangeChanged(Property* property, int minVal, int maxVal);
    void singleStepChanged(Property* property, int step);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<IntPropertyManagerPrivate> impl_;
    friend class IntPropertyManagerPrivate;
};
