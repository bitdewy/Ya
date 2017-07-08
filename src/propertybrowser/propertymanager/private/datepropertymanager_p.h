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
#include <QtCore/QDate>

class Property;
class DatePropertyManager;
class DatePropertyManagerPrivate
{
    DatePropertyManager* datePropertyManagerPtr_;
    friend class DatePropertyManager;
public:
    explicit DatePropertyManagerPrivate(DatePropertyManager* q);
    DatePropertyManagerPrivate(const DatePropertyManagerPrivate&) = delete;
    DatePropertyManagerPrivate& operator=(const DatePropertyManagerPrivate&) = delete;

    struct Data
    {
        QDate val{QDate::currentDate()};
        QDate minVal{QDate(1752, 9, 14)};
        QDate maxVal{QDate(7999, 12, 31)};
        QDate minimumValue() const;
        QDate maximumValue() const;
        void setMinimumValue(const QDate& newMinVal);
        void setMaximumValue(const QDate& newMaxVal);
    };

    QString format_;

    using PropertyValueMap = QMap<const Property*, Data>;
    QMap<const Property*, Data> values_;
};
