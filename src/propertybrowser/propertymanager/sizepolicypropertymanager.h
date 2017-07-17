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
class EnumPropertyManager;
class SizePolicyPropertyManagerPrivate;
class SizePolicyPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit SizePolicyPropertyManager(QObject* parent = nullptr);
    ~SizePolicyPropertyManager();

    IntPropertyManager* subIntPropertyManager() const;
    EnumPropertyManager* subEnumPropertyManager() const;

    QSizePolicy value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QSizePolicy& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QSizePolicy& val);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<SizePolicyPropertyManagerPrivate> impl_;
    friend class SizePolicyPropertyManagerPrivate;
};
