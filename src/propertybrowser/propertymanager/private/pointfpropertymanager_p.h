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
#include <QtCore/QPointF>

class Property;
class DoublePropertyManager;
class PointFPropertyManager;
class PointFPropertyManagerPrivate
{
    PointFPropertyManager* pointFPropertyManagerPtr_;
    friend class PointFPropertyManager;
public:

    struct Data
    {
        QPointF val;
        int decimals{2};
    };

    void slotDoubleChanged(Property* property, double value);
    void slotPropertyDestroyed(Property* property);

    using PropertyValueMap = QMap<const Property*, Data>;
    PropertyValueMap values_;

    DoublePropertyManager* doublePropertyManager_;

    QMap<const Property*, Property*> propertyToX_;
    QMap<const Property*, Property*> propertyToY_;
    QMap<const Property*, Property*> xToProperty_;
    QMap<const Property*, Property*> yToProperty_;
};
