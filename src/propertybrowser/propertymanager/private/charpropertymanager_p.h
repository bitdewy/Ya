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

class Property;
class CharPropertyManager;
class CharPropertyManagerPrivate
{
    CharPropertyManager* charPropertyManagerPtr_;
    friend class CharPropertyManager;
public:

    typedef QMap<const Property*, QChar> PropertyValueMap;
    PropertyValueMap values_;
};
