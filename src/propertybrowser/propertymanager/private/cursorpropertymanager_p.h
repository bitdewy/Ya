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

class QCursor;
class Property;
class CursorPropertyManager;
class CursorPropertyManagerPrivate
{
    CursorPropertyManager* cursorPropertyManagerPtr_;
    friend class CursorPropertyManager;
public:
    using PropertyValueMap = QMap<const Property*, QCursor>;
    PropertyValueMap values_;
};
