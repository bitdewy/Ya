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

class QColor;
class Property;
class IntPropertyManager;
class ColorPropertyManager;
class ColorPropertyManagerPrivate
{
    ColorPropertyManager* colorPropertyManagerPtr_;
    friend class ColorPropertyManager;
public:
    ColorPropertyManagerPrivate();
    ~ColorPropertyManagerPrivate();
    ColorPropertyManagerPrivate(const ColorPropertyManagerPrivate&) = delete;
    ColorPropertyManagerPrivate& operator=(const ColorPropertyManagerPrivate&) = delete;

    void slotIntChanged(Property* property, int value);
    void slotPropertyDestroyed(Property* property);

    using PropertyValueMap = QMap<const Property*, QColor>;
    PropertyValueMap values_;

    IntPropertyManager* intPropertyManager_;

    QMap<const Property*, Property*> propertyToR_;
    QMap<const Property*, Property*> propertyToG_;
    QMap<const Property*, Property*> propertyToB_;
    QMap<const Property*, Property*> propertyToA_;

    QMap<const Property*, Property*> rToProperty_;
    QMap<const Property*, Property*> gToProperty_;
    QMap<const Property*, Property*> bToProperty_;
    QMap<const Property*, Property*> aToProperty_;
};
