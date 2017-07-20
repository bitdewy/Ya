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
#include <QtCore/QSizeF>

class Property;
class DoublePropertyManager;
class SizeFPropertyManager;
class SizeFPropertyManagerPrivate
{
    SizeFPropertyManager* sizeFPropertyManagerPtr_;
    friend class SizeFPropertyManager;
public:

    void slotDoubleChanged(Property* property, double value);
    void slotPropertyDestroyed(Property* property);
    void setValue(Property* property, const QSizeF& val);
    void setRange(Property* property, const QSizeF& minVal, const QSizeF& maxVal, const QSizeF& val);

    struct Data
    {
        QSizeF val{0, 0};
        QSizeF minVal{0, 0};
        QSizeF maxVal{INT_MAX, INT_MAX};
        int decimals{2};
        QSizeF minimumValue() const;
        QSizeF maximumValue() const;
        void setMinimumValue(const QSizeF& newMinVal);
        void setMaximumValue(const QSizeF& newMaxVal);
    };

    using PropertyValueMap = QMap<const Property*, Data>;
    PropertyValueMap values_;

    DoublePropertyManager* doublePropertyManager_;

    QMap<const Property*, Property*> propertyToW_;
    QMap<const Property*, Property*> propertyToH_;

    QMap<const Property*, Property*> wToProperty_;
    QMap<const Property*, Property*> hToProperty_;
};
