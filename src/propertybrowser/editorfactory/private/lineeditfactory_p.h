/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include "editorfactory_p.h"
#include <QtWidgets/QLineEdit>

class LineEditFactory;
class LineEditFactoryPrivate : public EditorFactoryPrivate<QLineEdit>
{
    LineEditFactory* lineEditFactoryPtr_;
    friend class LineEditFactory;
public:

    void slotPropertyChanged(Property* property, const QString& value);
    void slotRegExpChanged(Property* property, const QRegExp& regExp);
    void slotSetValue(const QString& value);
};
