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
class IntPropertyManager;
class IntPropertyManagerPrivate
{
    IntPropertyManager* intPropertyManagerPtr_;
    friend class IntPropertyManager;
public:

    struct Data
    {
        int val{0};
        int minVal{-INT_MAX};
        int maxVal{INT_MAX};
        int singleStep{1};
        int minimumValue() const;
        int maximumValue() const;
        void setMinimumValue(int newMinVal);
        void setMaximumValue(int newMaxVal);
    };

    using PropertyValueMap = QMap<const Property*, Data>;
    PropertyValueMap values_;
};
