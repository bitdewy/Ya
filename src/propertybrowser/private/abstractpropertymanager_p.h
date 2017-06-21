/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QSet>

class Property;
class AbstractPropertyManager;
class AbstractPropertyManagerPrivate
{
    friend class AbstractPropertyManager;
    AbstractPropertyManager* propertyManagerPtr_;
public:
    AbstractPropertyManagerPrivate();
    ~AbstractPropertyManagerPrivate();
    AbstractPropertyManagerPrivate(const AbstractPropertyManagerPrivate&) = delete;
    AbstractPropertyManagerPrivate& operator=(const AbstractPropertyManagerPrivate&) = delete;

    void propertyDestroyed(Property* property);
    void propertyChanged(Property* property) const;
    void propertyRemoved(Property* property, Property* parentProperty) const;
    void propertyInserted(Property* property,
                          Property* parentProperty,
                          Property* afterProperty) const;
private:
    QSet<Property*> properties_;
};
