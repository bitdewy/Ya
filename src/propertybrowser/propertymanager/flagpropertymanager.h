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

class BoolPropertyManager;
class FlagPropertyManagerPrivate;
class FlagPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit FlagPropertyManager(QObject* parent = nullptr);
    ~FlagPropertyManager();

    BoolPropertyManager* subBoolPropertyManager() const;

    int value(const Property* property) const;
    QStringList flagNames(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, int val);
    void setFlagNames(Property* property, const QStringList& names);
Q_SIGNALS:
    void valueChanged(Property* property, int val);
    void flagNamesChanged(Property* property, const QStringList& names);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<FlagPropertyManagerPrivate> impl_;
    friend class FlagPropertyManagerPrivate;
};
