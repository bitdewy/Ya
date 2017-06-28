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
class EnumPropertyManager;
class EnumPropertyManagerPrivate
{
    EnumPropertyManager* EnumPropertyManagerPtr_;
    friend class EnumPropertyManager;
public:

    struct Data
    {
        int val{-1};
        QStringList enumNames;
        QMap<int, QIcon> enumIcons;
    };

    using PropertyValueMap = QMap<const Property*, Data>;
    PropertyValueMap values_;
};
