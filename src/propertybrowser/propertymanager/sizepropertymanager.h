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

class IntPropertyManager;
class SizePropertyManagerPrivate;
class SizePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit SizePropertyManager(QObject* parent = nullptr);
    ~SizePropertyManager();

    IntPropertyManager* subIntPropertyManager() const;

    QSize value(const Property* property) const;
    QSize minimum(const Property* property) const;
    QSize maximum(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QSize& val);
    void setMinimum(Property* property, const QSize& minVal);
    void setMaximum(Property* property, const QSize& maxVal);
    void setRange(Property* property, const QSize& minVal, const QSize& maxVal);
Q_SIGNALS:
    void valueChanged(Property* property, const QSize& val);
    void rangeChanged(Property* property, const QSize& minVal, const QSize& maxVal);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<SizePropertyManagerPrivate> impl_;
    friend class SizePropertyManagerPrivate;
};
