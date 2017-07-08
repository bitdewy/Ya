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
#include <QtWidgets/QComboBox>

class EnumEditorFactory;
class EnumEditorFactoryPrivate : public EditorFactoryPrivate<QComboBox>
{
    EnumEditorFactory* enumEditorFactoryPtr_;
    friend class EnumEditorFactory;
public:

    void slotPropertyChanged(Property* property, int value);
    void slotEnumNamesChanged(Property* property, const QStringList&);
    void slotEnumIconsChanged(Property* property, const QMap<int, QIcon>&);
    void slotSetValue(int value);
};
