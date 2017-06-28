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

class EnumPropertyManagerPrivate;
class EnumPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit EnumPropertyManager(QObject* parent = nullptr);
    ~EnumPropertyManager();

    int value(const Property* property) const;
    QStringList enumNames(const Property* property) const;
    QMap<int, QIcon> enumIcons(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, int val);
    void setEnumNames(Property* property, const QStringList& names);
    void setEnumIcons(Property* property, const QMap<int, QIcon>& icons);
Q_SIGNALS:
    void valueChanged(Property* property, int val);
    void enumNamesChanged(Property* property, const QStringList& names);
    void enumIconsChanged(Property* property, const QMap<int, QIcon>& icons);
protected:
    QString valueText(const Property* property) const;
    QIcon valueIcon(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<EnumPropertyManagerPrivate> impl_;
    friend class EnumPropertyManagerPrivate;
};
