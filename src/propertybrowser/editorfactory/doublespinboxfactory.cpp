/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "doublespinboxfactory.h"
#include "private/doublespinboxfactory_p.h"

DoubleSpinBoxFactory::DoubleSpinBoxFactory(QObject* parent) :
    AbstractEditorFactory<DoublePropertyManager>(parent),
    impl_(new DoubleSpinBoxFactoryPrivate())
{
    impl_->doubleSpinBoxFactoryPtr_ = this;

}

DoubleSpinBoxFactory::~DoubleSpinBoxFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void DoubleSpinBoxFactory::connectPropertyManager(DoublePropertyManager* manager)
{
    connect(manager, &DoublePropertyManager::valueChanged,
        this, &DoubleSpinBoxFactory::slotPropertyChanged);
    connect(manager, &DoublePropertyManager::rangeChanged,
        this, &DoubleSpinBoxFactory::slotRangeChanged);
    connect(manager, &DoublePropertyManager::singleStepChanged,
        this, &DoubleSpinBoxFactory::slotSingleStepChanged);
    connect(manager, &DoublePropertyManager::decimalsChanged,
        this, &DoubleSpinBoxFactory::slotDecimalsChanged);
}

QWidget* DoubleSpinBoxFactory::createEditor(DoublePropertyManager* manager,
    Property* property, QWidget* parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setSingleStep(manager->singleStep(property));
    editor->setDecimals(manager->decimals(property));
    editor->setRange(manager->minimum(property), manager->maximum(property));
    editor->setValue(manager->value(property));
    editor->setKeyboardTracking(false);

    connect(editor, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, [this](double value) { impl_->slotSetValue(value); });
    connect(editor, &QDoubleSpinBox::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void DoubleSpinBoxFactory::disconnectPropertyManager(DoublePropertyManager* manager)
{
    disconnect(manager, &DoublePropertyManager::valueChanged,
        this, &DoubleSpinBoxFactory::slotPropertyChanged);
    disconnect(manager, &DoublePropertyManager::rangeChanged,
        this, &DoubleSpinBoxFactory::slotRangeChanged);
    disconnect(manager, &DoublePropertyManager::singleStepChanged,
        this, &DoubleSpinBoxFactory::slotSingleStepChanged);
    disconnect(manager, &DoublePropertyManager::decimalsChanged,
        this, &DoubleSpinBoxFactory::slotDecimalsChanged);
}

void DoubleSpinBoxFactory::slotPropertyChanged(Property* property, double value)
{
    impl_->slotPropertyChanged(property, value);
}

void DoubleSpinBoxFactory::slotRangeChanged(Property* property, double min, double max)
{
    impl_->slotRangeChanged(property, min, max);
}

void DoubleSpinBoxFactory::slotSingleStepChanged(Property* property, double step)
{
    impl_->slotSingleStepChanged(property, step);
}

void DoubleSpinBoxFactory::slotDecimalsChanged(Property* property, int prec)
{
    impl_->slotDecimalsChanged(property, prec);
}
