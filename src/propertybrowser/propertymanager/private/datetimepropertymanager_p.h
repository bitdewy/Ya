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
#include <QtCore/QDateTime>

class Property;
class DateTimePropertyManager;
class DateTimePropertyManagerPrivate
{
    DateTimePropertyManager* dateTimePropertyManagerPtr_;
    friend class DateTimePropertyManager;
public:
    explicit DateTimePropertyManagerPrivate(DateTimePropertyManager* q);

    const QString format_;

    using  PropertyValueMap = QMap<const Property*, QDateTime>;
    PropertyValueMap values_;
};
