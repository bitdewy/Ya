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
class BoolPropertyManager;
class FlagPropertyManager;
class FlagPropertyManagerPrivate
{
    FlagPropertyManager* flagPropertyManagerPtr_;
    friend class FlagPropertyManager;
public:
    FlagPropertyManagerPrivate();
    FlagPropertyManagerPrivate(const FlagPropertyManagerPrivate&) = delete;
    FlagPropertyManagerPrivate operator=(const FlagPropertyManagerPrivate&) = delete;

    void slotBoolChanged(Property* property, bool value);
    void slotPropertyDestroyed(Property* property);

    struct Data
    {
        int val{-1};
        QStringList flagNames;
    };

    using PropertyValueMap = QMap<const Property*, Data> ;
    PropertyValueMap values_;

    BoolPropertyManager* boolPropertyManager_;

    QMap<const Property*, QList<Property*>> propertyToFlags_;

    QMap<const Property*, Property*> flagToProperty_;
};
