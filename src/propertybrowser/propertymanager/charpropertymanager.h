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

class CharPropertyManagerPrivate;
class CharPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit CharPropertyManager(QObject* parent = nullptr);
    ~CharPropertyManager();

    QChar value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QChar& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QChar& val);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<CharPropertyManagerPrivate> impl_;
    friend class CharPropertyManagerPrivate;
};
