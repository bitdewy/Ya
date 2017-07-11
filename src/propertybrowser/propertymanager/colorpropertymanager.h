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
class ColorPropertyManagerPrivate;
class ColorPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit ColorPropertyManager(QObject* parent = nullptr);
    ~ColorPropertyManager();

    IntPropertyManager* subIntPropertyManager() const;

    QColor value(const Property* property) const;

public Q_SLOTS:
    void setValue(Property* property, const QColor& val);
Q_SIGNALS:
    void valueChanged(Property* property, const QColor& val);
protected:
    QString valueText(const Property* property) const;
    QIcon valueIcon(const Property* property) const;
    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
private:
    QScopedPointer<ColorPropertyManagerPrivate> impl_;
    friend class ColorPropertyManager;
};
