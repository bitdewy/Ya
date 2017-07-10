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

class DateTimeEditFactory;
class DateTimeEditFactoryPrivate : public EditorFactoryPrivate<QDateTimeEdit>
{
    DateTimeEditFactory* dateTimeEditPropertyPtr_;
    friend class DateTimeEditFactory;
public:

    void slotPropertyChanged(Property* property, const QDateTime& value);
    void slotSetValue(const QDateTime& value);

};
