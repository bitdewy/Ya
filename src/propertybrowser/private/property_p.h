/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QList>
#include <QtCore/QSet>

class AbstractPropertyManager;
class Property;
class PropertyPrivate
{
public:
    PropertyPrivate(AbstractPropertyManager* manager);
    Property* propertyPtr_;

    QSet<Property*> parentItems_;
    QList<Property*> subItems_;

    QString toolTip_;
    QString descriptionToolTip_;
    QString statusTip_;
    QString whatsThis_;
    QString name_;
    bool enabled_;
    bool modified_;

    AbstractPropertyManager* const manager_;
};
