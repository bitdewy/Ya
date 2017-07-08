/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "datepropertymanager_p.h"
#include "private/propertybrowserutils_p.h"
#include "propertymanagerutils_p.h"

DatePropertyManagerPrivate::DatePropertyManagerPrivate(DatePropertyManager* q) :
    datePropertyManagerPtr_(q),
    format_(PropertyBrowserUtils::dateFormat())
{
}


QDate DatePropertyManagerPrivate::Data::minimumValue() const
{
    return minVal;
}

QDate DatePropertyManagerPrivate::Data::maximumValue() const
{
    return maxVal;
}

void DatePropertyManagerPrivate::Data::setMinimumValue(const QDate& newMinVal)
{
    setSimpleMinimumData(this, newMinVal);
}

void DatePropertyManagerPrivate::Data::setMaximumValue(const QDate& newMaxVal)
{
    setSimpleMaximumData(this, newMaxVal);
}
