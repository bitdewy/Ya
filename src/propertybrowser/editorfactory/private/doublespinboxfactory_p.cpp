/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "doublespinboxfactory_p.h"
#include "editorfactory/doublespinboxfactory.h"

void DoubleSpinBoxFactoryPrivate::slotPropertyChanged(Property* property, double value)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.cend())
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        if (editor->value() != value)
        {
            editor->blockSignals(true);
            editor->setValue(value);
            editor->blockSignals(false);
        }
    }
}

void DoubleSpinBoxFactoryPrivate::slotRangeChanged(Property* property,
    double min, double max)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.cend())
    {
        return;
    }
    auto manager = doubleSpinBoxFactoryPtr_->propertyManager(property);
    if (!manager)
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setRange(min, max);
        editor->setValue(manager->value(property));
        editor->blockSignals(false);
    }
}

void DoubleSpinBoxFactoryPrivate::slotSingleStepChanged(Property* property, double step)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.cend())
    {
        return;
    }
    auto manager = doubleSpinBoxFactoryPtr_->propertyManager(property);
    if (!manager)
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setSingleStep(step);
        editor->blockSignals(false);
    }
}

void DoubleSpinBoxFactoryPrivate::slotDecimalsChanged(Property* property, int prec)
{
    const auto it = createdEditors_.constFind(property);
    if (it == createdEditors_.constEnd())
    {
        return;
    }
    auto manager = doubleSpinBoxFactoryPtr_->propertyManager(property);
    if (!manager)
    {
        return;
    }
    for (auto&& editor : it.value())
    {
        editor->blockSignals(true);
        editor->setDecimals(prec);
        editor->setValue(manager->value(property));
        editor->blockSignals(false);
    }
}

void DoubleSpinBoxFactoryPrivate::slotSetValue(double value)
{
    auto object = doubleSpinBoxFactoryPtr_->sender();
    const auto itcend = editorToProperty_.constEnd();
    for (auto itEditor = editorToProperty_.constBegin(); itEditor != itcend; ++itEditor)
    {
        if (itEditor.key() == object)
        {
            auto property = itEditor.value();
            auto manager = doubleSpinBoxFactoryPtr_->propertyManager(property);
            if (!manager)
            {
                return;
            }
            manager->setValue(property, value);
            return;
        }
    }
}
