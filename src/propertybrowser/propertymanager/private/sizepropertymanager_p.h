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
#include <QtCore/QSize>

class Property;
class IntPropertyManager;
class SizePropertyManager;
class SizePropertyManagerPrivate
{
    SizePropertyManager* sizePropertyManagerPtr_;
    friend class SizePropertyManager;
public:

    void slotIntChanged(Property* property, int value);
    void slotPropertyDestroyed(Property* property);
    void setValue(Property* property, const QSize& val);
    void setRange(Property* property, const QSize& minVal, const QSize& maxVal, const QSize& val);

    struct Data
    {
        QSize val;
        QSize minVal;
        QSize maxVal;
        Data();
        QSize minimumValue() const;
        QSize maximumValue() const;
        void setMinimumValue(const QSize& newMinVal);
        void setMaximumValue(const QSize& newMaxVal);
    };

    using PropertyValueMap = QMap<const Property*, Data>;
    PropertyValueMap values_;

    IntPropertyManager* intPropertyManager_;

    QMap<const Property*, Property*> propertyToW_;
    QMap<const Property*, Property*> propertyToH_;

    QMap<const Property*, Property*> wToProperty_;
    QMap<const Property*, Property*> hToProperty_;
};
