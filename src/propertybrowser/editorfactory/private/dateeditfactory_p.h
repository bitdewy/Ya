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
#include <QtWidgets/QDateTimeEdit>

class DateEditFactory;
class DateEditFactoryPrivate : public EditorFactoryPrivate<QDateEdit>
{
    DateEditFactory* dateEditFactoryPtr_;
    friend class DateEditFactory;
public:

    void slotPropertyChanged(Property* property, const QDate& value);
    void slotRangeChanged(Property* property, const QDate& min, const QDate& max);
    void slotSetValue(const QDate& value);
};
