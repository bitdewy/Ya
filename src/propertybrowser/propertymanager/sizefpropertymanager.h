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
class SizeFPropertyManagerPrivate;
class SizeFPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit SizeFPropertyManager(QObject* parent = nullptr);
    ~SizeFPropertyManager();

    DoublePropertyManager* subDoublePropertyManager() const;

    QSizeF value(const Property* property) const;
    QSizeF minimum(const Property* property) const;
    QSizeF maximum(const Property* property) const;
    int decimals(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QSizeF& val);
    void setMinimum(Property* property, const QSizeF& minVal);
    void setMaximum(Property* property, const QSizeF& maxVal);
    void setRange(Property* property, const QSizeF& minVal, const QSizeF& maxVal);
    void setDecimals(Property* property, int prec);
Q_SIGNALS:
    void valueChanged(Property* property, const QSizeF& val);
    void rangeChanged(Property* property, const QSizeF& minVal, const QSizeF& maxVal);
    void decimalsChanged(Property* property, int prec);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<SizeFPropertyManagerPrivate> impl_;
    friend class SizeFPropertyManagerPrivate;
};

