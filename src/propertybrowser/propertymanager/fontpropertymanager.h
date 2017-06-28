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
class BoolPropertyManager;
class FontPropertyManagerPrivate;
class FontPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit FontPropertyManager(QObject* parent = nullptr);
    ~FontPropertyManager();

    IntPropertyManager* subIntPropertyManager() const;
    EnumPropertyManager* subEnumPropertyManager() const;
    BoolPropertyManager* subBoolPropertyManager() const;

    QFont value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QFont& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QFont& val);
protected:
    QString valueText(const Property* property) const;
    QIcon valueIcon(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<FontPropertyManagerPrivate> impl_;
    friend class FontPropertyManagerPrivate;
};
