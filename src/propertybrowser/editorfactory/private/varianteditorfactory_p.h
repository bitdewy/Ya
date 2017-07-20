/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QMap>

class VariantEditorFactory;
class SpinBoxFactory;
class DoubleSpinBoxFactory;
class CheckBoxFactory;
class LineEditFactory;
class DateEditFactory;
class TimeEditFactory;
class DateTimeEditFactory;
class KeySequenceEditorFactory;
class AbstractEditorFactoryBase;
class CharEditorFactory;
class EnumEditorFactory;
class CursorEditorFactory;
class ColorEditorFactory;
class FontEditorFactory;
class VariantEditorFactoryPrivate
{
    VariantEditorFactory* variantEditorFactoryPtr_;
    friend class  VariantEditorFactory;
public:

    SpinBoxFactory* spinBoxFactory_;
    DoubleSpinBoxFactory* doubleSpinBoxFactory_;
    CheckBoxFactory* checkBoxFactory_;
    LineEditFactory* lineEditFactory_;
    DateEditFactory* dateEditFactory_;
    TimeEditFactory* timeEditFactory_;
    DateTimeEditFactory* dateTimeEditFactory_;
    KeySequenceEditorFactory* keySequenceEditorFactory_;
    CharEditorFactory* charEditorFactory_;
    EnumEditorFactory* comboBoxFactory_;
    CursorEditorFactory* cursorEditorFactory_;
    ColorEditorFactory* colorEditorFactory_;
    FontEditorFactory* fontEditorFactory_;

    QMap<AbstractEditorFactoryBase*, int> factoryToType_;
    QMap<int, AbstractEditorFactoryBase*> typeToFactory_;
};
