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
class DoublePropertyManager;
class DoublePropertyManagerPrivate
{
    DoublePropertyManager* doublePropertyManagerPtr_;
    friend class DoublePropertyManager;
public:

    struct Data
    {
        double val{0};
        double minVal{-DBL_MAX};
        double maxVal{DBL_MAX};
        double singleStep{1};
        int decimals{2};
        double minimumValue() const;
        double maximumValue() const;
        void setMinimumValue(double newMinVal);
        void setMaximumValue(double newMaxVal);
    };

    using PropertyValueMap = QMap<const Property*, Data>;
    PropertyValueMap values_;
};
