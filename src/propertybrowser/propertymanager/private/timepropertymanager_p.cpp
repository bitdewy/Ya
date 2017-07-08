/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "timepropertymanager_p.h"
#include "private/propertybrowserutils_p.h"

TimePropertyManagerPrivate::TimePropertyManagerPrivate(TimePropertyManager *q) :
    timePropertyManagerPtr_(q),
    format_(PropertyBrowserUtils::timeFormat())
{
}
