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

class GroupPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT
public:
    explicit GroupPropertyManager(QObject* parent = nullptr);
    ~GroupPropertyManager();

protected:
    virtual bool hasValue(const Property* property) const;

    virtual void initializeProperty(Property* property);
    virtual void uninitializeProperty(Property* property);
};
