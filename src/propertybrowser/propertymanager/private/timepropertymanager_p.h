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
#include <QtCore/QTime>

class Property;
class TimePropertyManager;
class TimePropertyManagerPrivate
{
    TimePropertyManager* timePropertyManagerPtr_;
    friend class TimePropertyManager;
public:
    explicit TimePropertyManagerPrivate(TimePropertyManager* q);

    const QString format_;

    using PropertyValueMap = QMap<const Property*, QTime>;
    PropertyValueMap values_;
};
