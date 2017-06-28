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

class BoolPropertyManagerPrivate;

class BoolPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit BoolPropertyManager(QObject* parent = nullptr);
    ~BoolPropertyManager();

    bool value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, bool val);
Q_SIGNALS:
    void valueChanged(Property* property, bool val);
protected:
    QString valueText(const Property* property) const;
    QIcon valueIcon(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<BoolPropertyManagerPrivate> impl_;
    friend class BoolPropertyManager;
};
