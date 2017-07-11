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

class CursorPropertyManagerPrivate;
class CursorPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit CursorPropertyManager(QObject* parent = nullptr);
    ~CursorPropertyManager();

    QCursor value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QCursor& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QCursor& val);
protected:
    QString valueText(const Property* property) const;
    QIcon valueIcon(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<CursorPropertyManagerPrivate> impl_;
    friend class CursorPropertyManagerPrivate;
};
