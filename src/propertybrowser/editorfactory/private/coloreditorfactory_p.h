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
#include "coloreditwidget.h"

class ColorEditorFactory;
class ColorEditorFactoryPrivate : public EditorFactoryPrivate<ColorEditWidget>
{
    ColorEditorFactory* colorEditorFactoryPtr_;
    friend class ColorEditorFactory;
public:

    void slotPropertyChanged(Property* property, const QColor& value);
    void slotSetValue(const QColor& value);
};
