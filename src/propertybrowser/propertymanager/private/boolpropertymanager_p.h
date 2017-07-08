/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QMap>
#include <QtGui/QIcon>

class Property;
class BoolPropertyManager;
class BoolPropertyManagerPrivate
{
    BoolPropertyManager* boolPropertyManagerPtr_;
    friend class BoolPropertyManager;
public:
    BoolPropertyManagerPrivate();
    BoolPropertyManagerPrivate(const BoolPropertyManagerPrivate&) = delete;
    BoolPropertyManagerPrivate& operator=(const BoolPropertyManagerPrivate&) = delete;

    QMap<const Property*, bool> values_;
    const QIcon checkedIcon_;
    const QIcon uncheckedIcon_;
};
