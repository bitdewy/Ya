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
#include <QtCore/QRectF>

class Property;
class DoublePropertyManager;
class RectFPropertyManager;
class RectFPropertyManagerPrivate
{
    RectFPropertyManager* rectFPropertyManagerPtr_;
    friend class RectFPropertyManager;
public:

    void slotDoubleChanged(Property* property, double value);
    void slotPropertyDestroyed(Property* property);
    void setConstraint(Property* property, const QRectF& constraint, const QRectF& val);

    struct Data
    {
        QRectF val{0, 0, 0, 0};
        QRectF constraint;
        int decimals{2};
    };

    typedef QMap<const Property*, Data> PropertyValueMap;
    PropertyValueMap values_;

    DoublePropertyManager* doublePropertyManager_;

    QMap<const Property*, Property*> propertyToX_;
    QMap<const Property*, Property*> propertyToY_;
    QMap<const Property*, Property*> propertyToW_;
    QMap<const Property*, Property*> propertyToH_;

    QMap<const Property*, Property*> xToProperty_;
    QMap<const Property*, Property*> yToProperty_;
    QMap<const Property*, Property*> wToProperty_;
    QMap<const Property*, Property*> hToProperty_;
};
