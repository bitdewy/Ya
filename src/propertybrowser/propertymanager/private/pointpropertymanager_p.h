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

class QPoint;
class Property;
class IntPropertyManager;
class PointPropertyManager;
class PointPropertyManagerPrivate
{
    PointPropertyManager* pointPropertyManagerPtr_;
    friend class PointPropertyManager;
public:
    PointPropertyManagerPrivate();
    PointPropertyManagerPrivate(const PointPropertyManagerPrivate&) = delete;
    PointPropertyManagerPrivate operator=(const PointPropertyManagerPrivate&) = delete;

    void slotIntChanged(Property* property, int value);
    void slotPropertyDestroyed(Property* property);

    using PropertyValueMap = QMap<const Property*, QPoint>;
    PropertyValueMap values_;

    IntPropertyManager* intPropertyManager_;

    QMap<const Property*, Property*> propertyToX_;
    QMap<const Property*, Property*> propertyToY_;

    QMap<const Property*, Property*> xToProperty_;
    QMap<const Property*, Property*> yToProperty_;
};
