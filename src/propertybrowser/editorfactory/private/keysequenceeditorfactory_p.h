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
#include <QtWidgets/QKeySequenceEdit>

class KeySequenceEditorFactory;
class KeySequenceEditorFactoryPrivate : public EditorFactoryPrivate<QKeySequenceEdit>
{
    KeySequenceEditorFactory* keySequenceEditorFactoryPtr_;
    friend class KeySequenceEditorFactory;

public:
    void slotPropertyChanged(Property* property, const QKeySequence& value);
    void slotSetValue(const QKeySequence& value);
};
