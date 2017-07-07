/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "datetimepropertymanager_p.h"
#include "private/propertybrowserutils_p.h"

DateTimePropertyManagerPrivate::DateTimePropertyManagerPrivate(DateTimePropertyManager* q) :
    dateTimePropertyManagerPtr_(q),
    format_(PropertyBrowserUtils::dateTimeFormat())
{
}

