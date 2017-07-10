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

class QKeySequence;
class Property;
class KeySequencePropertyManager;
class KeySequencePropertyManagerPrivate
{
    KeySequencePropertyManager* keySequencePropertyManagerPtr_;
    friend class KeySequencePropertyManager;
public:

    QString format_;

    using PropertyValueMap = QMap<const Property*, QKeySequence>;
    PropertyValueMap values_;
};
