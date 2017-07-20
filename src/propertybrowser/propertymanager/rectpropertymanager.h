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
class RectPropertyManagerPrivate;
class RectPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit RectPropertyManager(QObject* parent = nullptr);
    ~RectPropertyManager();

    IntPropertyManager* subIntPropertyManager() const;

    QRect value(const Property* property) const;
    QRect constraint(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QRect& val);
    void setConstraint(Property* property, const QRect& constraint);
Q_SIGNALS:
    void valueChanged(Property* property, const QRect& val);
    void constraintChanged(Property* property, const QRect& constraint);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<RectPropertyManagerPrivate> impl_;
    friend class RectPropertyManagerPrivate;
};
