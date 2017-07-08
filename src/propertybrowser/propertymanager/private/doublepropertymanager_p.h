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
        Data();
        double val;
        double minVal;
        double maxVal;
        double singleStep;
        int decimals;
        double minimumValue() const;
        double maximumValue() const;
        void setMinimumValue(double newMinVal);
        void setMaximumValue(double newMaxVal);
    };

    using PropertyValueMap = QMap<const Property*, Data>;
    PropertyValueMap values_;
};
