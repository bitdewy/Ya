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

class EnumPropertyManager;
class LocalePropertyManagerPrivate;
class LocalePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit LocalePropertyManager(QObject* parent = nullptr);
    ~LocalePropertyManager();

    EnumPropertyManager* subEnumPropertyManager() const;

    QLocale value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QLocale& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QLocale& val);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<LocalePropertyManagerPrivate> impl_;
    friend class LocalePropertyManagerPrivate;
};
