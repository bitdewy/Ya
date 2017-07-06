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
#include <QtWidgets/QTimeEdit>

class TimeEditFactory;
class TimeEditFactoryPrivate : public EditorFactoryPrivate<QTimeEdit>
{
    TimeEditFactory* timeEditFactoryPtr_;
    friend class TimeEditFactory;
public:

    void slotPropertyChanged(Property* property, const QTime& value);
    void slotSetValue(const QTime& value);
};
