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

class StringPropertyManagerPrivate;
class StringPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit StringPropertyManager(QObject* parent = nullptr);
    ~StringPropertyManager();

    QString value(const Property* property) const;
    QRegExp regExp(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QString& val);
    void setRegExp(Property* property, const QRegExp& regExp);
Q_SIGNALS:
    void valueChanged(Property* property, const QString& val);
    void regExpChanged(Property* property, const QRegExp& regExp);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<StringPropertyManagerPrivate> impl_;
    friend class StringPropertyManagerPrivate;
};
