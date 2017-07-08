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
#include "chareditwidget.h"

class CharEditorFactory;
class CharEditorFactoryPrivate : public EditorFactoryPrivate<CharEditWidget>
{
    CharEditorFactory* charEditorFactoryPtr_;
    friend class CharEditorFactory;
public:

    void slotPropertyChanged(Property* property, const QChar& value);
    void slotSetValue(const QChar& value);

};
