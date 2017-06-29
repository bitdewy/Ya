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
#include "booleditwidget.h"

class CheckBoxFactory;
class CheckBoxFactoryPrivate : public EditorFactoryPrivate<BoolEditWidget>
{
    CheckBoxFactory* checkBoxFactoryPtr_;
    friend class CheckBoxFactory;
public:
    void slotPropertyChanged(Property* property, bool value);
    void slotSetValue(bool value);
};

