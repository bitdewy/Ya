/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "doublepropertymanager_p.h"
#include "propertymanager/private/propertymanagerutils_p.h"

double DoublePropertyManagerPrivate::Data::minimumValue() const
{
    return minVal;
}

double DoublePropertyManagerPrivate::Data::maximumValue() const
{
    return maxVal;
}

void DoublePropertyManagerPrivate::Data::setMinimumValue(double newMinVal)
{
    setSimpleMinimumData(this, newMinVal);
}

void DoublePropertyManagerPrivate::Data::setMaximumValue(double newMinVal)
{
    setSimpleMaximumData(this, newMinVal);
}
