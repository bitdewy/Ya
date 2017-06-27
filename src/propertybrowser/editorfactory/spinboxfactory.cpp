/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#include "spinboxfactory.h"
#include "private/spinboxfactory_p.h"

SpinBoxFactory::SpinBoxFactory(QObject *parent) :
    AbstractEditorFactory<IntPropertyManager>(parent),
    impl_(new SpinBoxFactoryPrivate())
{
    impl_->spinBoxFactoryPtr_ = this;

}

SpinBoxFactory::~SpinBoxFactory()
{
    qDeleteAll(impl_->editorToProperty_.keys());
}

void SpinBoxFactory::connectPropertyManager(IntPropertyManager* manager)
{
    connect(manager, &IntPropertyManager::valueChanged,
        this, &SpinBoxFactory::slotPropertyChanged);
    connect(manager, &IntPropertyManager::rangeChanged,
        this, &SpinBoxFactory::slotRangeChanged);
    connect(manager, &IntPropertyManager::singleStepChanged,
        this, &SpinBoxFactory::slotSingleStepChanged);
}

QWidget* SpinBoxFactory::createEditor(IntPropertyManager* manager,
        Property* property, QWidget *parent)
{
    auto editor = impl_->createEditor(property, parent);
    editor->setSingleStep(manager->singleStep(property));
    editor->setRange(manager->minimum(property), manager->maximum(property));
    editor->setValue(manager->value(property));
    editor->setKeyboardTracking(false);

    connect(editor, QOverload<int>::of(&QSpinBox::valueChanged),
        this, [this](int value) { impl_->slotSetValue(value); });
    connect(editor, &QSpinBox::destroyed,
        this, [this](QObject* o) { impl_->slotEditorDestroyed(o); });
    return editor;
}

void SpinBoxFactory::disconnectPropertyManager(IntPropertyManager* manager)
{
    disconnect(manager, &IntPropertyManager::valueChanged,
        this, &SpinBoxFactory::slotPropertyChanged);
    disconnect(manager, &IntPropertyManager::rangeChanged,
        this, &SpinBoxFactory::slotRangeChanged);
    disconnect(manager, &IntPropertyManager::singleStepChanged,
        this, &SpinBoxFactory::slotSingleStepChanged);
}

void SpinBoxFactory::slotPropertyChanged(Property* property, int value)
{
    impl_->slotPropertyChanged(property, value);
}

void SpinBoxFactory::slotRangeChanged(Property* property, int min, int max)
{
    impl_->slotRangeChanged(property, min, max);
}

void SpinBoxFactory::slotSingleStepChanged(Property* property, int step)
{
    impl_->slotSingleStepChanged(property, step);
}
