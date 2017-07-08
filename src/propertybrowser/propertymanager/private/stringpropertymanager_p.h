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
class StringPropertyManager;
class StringPropertyManagerPrivate
{
    StringPropertyManager* stringPropertyManagerPtr_;
    friend class StringPropertyManager;
public:

    struct Data
    {
        Data();
        QString val;
        QRegExp regExp;
    };

    using PropertyValueMap = QMap<const Property*, Data>;
    QMap<const Property*, Data> values_;
};
