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
#include <QtCore/QRect>

class Property;
class RectPropertyManager;
class IntPropertyManager;
class RectPropertyManagerPrivate
{
    RectPropertyManager* rectPropertyManagerPtr_;
    friend class RectPropertyManager;
public:

    void slotIntChanged(Property* property, int value);
    void slotPropertyDestroyed(Property* property);
    void setConstraint(Property* property, const QRect& constraint, const QRect& val);

    struct Data
    {
        QRect val{0, 0, 0, 0};
        QRect constraint;
    };

    using PropertyValueMap = QMap<const Property*, Data>;
    PropertyValueMap values_;

    IntPropertyManager* intPropertyManager_;

    QMap<const Property*, Property*> propertyToX_;
    QMap<const Property*, Property*> propertyToY_;
    QMap<const Property*, Property*> propertyToW_;
    QMap<const Property*, Property*> propertyToH_;

    QMap<const Property*, Property*> xToProperty_;
    QMap<const Property*, Property*> yToProperty_;
    QMap<const Property*, Property*> wToProperty_;
    QMap<const Property*, Property*> hToProperty_;
};
