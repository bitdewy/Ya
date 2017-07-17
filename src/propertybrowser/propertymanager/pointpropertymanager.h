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
class PointPropertyManagerPrivate;
class PointPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit PointPropertyManager(QObject* parent = nullptr);
    ~PointPropertyManager();

    IntPropertyManager* subIntPropertyManager() const;

    QPoint value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QPoint& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QPoint& val);
protected:
    QString valueText(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<PointPropertyManagerPrivate> impl_;
    friend class PointPropertyManagerPrivate;
};
