/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "intpropertymanager_p.h"
#include "propertymanagerutils_p.h"

int IntPropertyManagerPrivate::Data::minimumValue() const
{
    return minVal;
}

int IntPropertyManagerPrivate::Data::maximumValue() const
{
    return maxVal;
}

void IntPropertyManagerPrivate::Data::setMinimumValue(int newMinVal)
{
    setSimpleMinimumData(this, newMinVal);
}

void IntPropertyManagerPrivate::Data::setMaximumValue(int newMinVal)
{
    setSimpleMaximumData(this, newMinVal);
}
