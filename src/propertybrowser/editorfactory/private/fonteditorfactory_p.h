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
#include "fonteditwidget.h"

class FontEditorFactory;
class FontEditorFactoryPrivate : public EditorFactoryPrivate<FontEditWidget>
{
    FontEditorFactory* fontEditorFactoryPtr_;
    friend class FontEditorFactory;

public:
    void slotPropertyChanged(Property* property, const QFont& value);
    void slotSetValue(const QFont& value);
};
